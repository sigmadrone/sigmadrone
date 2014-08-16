/*
 * pluginchain.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: svassilev
 */

#include "pluginchain.h"
#include "iopacket.h"

PluginChain::PluginChain() : m_pluginsStarted(false)
{
	pthread_rwlock_init(&m_lock,0);
}

PluginChain::~PluginChain()
{
	while (!m_chain.empty()) {
		PluginMap::iterator it = m_chain.begin();
		PluginContext* pluginCtx = it->second;
		int altitude = pluginCtx->GetMyAltitude();
		assert(pluginCtx);

		/*
		 * Call stop with DETACH flag set. This should result in a call to
		 * PluginRuntime::DetachPlugin, which should dispose off the plugin
		 */
		pluginCtx->m_plugin->Stop(IPlugin::FLAG_STOP_AND_DETACH);
		it = m_chain.find(altitude);
		if (0 != it->second) {
			printf("ERROR: plugin %s did not respond to Stop command, "
				"will unload it dirty", pluginCtx->m_plugin->GetName());
			PluginDetach(it->second);
		}
	}

	pthread_rwlock_destroy(&m_lock);
}

int PluginChain::PluginAttach(
	IPlugin* plugin,
	SdPluginAltitude desiredAltitude,
	const char* const attachAbove[],
	const char* const attachBelow[],
	IPluginRuntime** pluginRuntime)
{
	int ret = EINVAL;
	std::string lowerPlugins;
	std::string upperPlugins;
	SdPluginAltitude newAltitude = desiredAltitude;
	SdPluginAltitude altitudeLo = SD_ALTITUDE_INVALID;
	SdPluginAltitude altitudeHi = SD_ALTITUDE_INVALID;
	PluginContext* pluginContext = 0;

	if (!plugin || !pluginRuntime) {
		return ret;
	}

	*pluginRuntime = 0;

	if (newAltitude >= SD_ALTITUDE_GROUP_LOWER_FILTER &&
		plugin->GetDeviceId() < SD_DEVICEID_FILTER) {
		newAltitude = SD_ALTITUDE_GROUP_DEVICE;
	}
	if (0 == attachAbove && 0 == attachBelow &&
		!(newAltitude >= SD_ALTITUDE_FIRST && newAltitude <= SD_ALTITUDE_LAST)) {
		return ret;
	}

	MergeNameList(&lowerPlugins, attachAbove);
	MergeNameList(&upperPlugins, attachBelow);

	LockWrite();

	/*
	 * Find nearest lower and high plugin altitudes, while honoring the
	 * provided dependencies
	 */
	if (lowerPlugins.length()>0) {
		for (PluginMapIt it = m_chain.begin(); it != m_chain.end(); ++it) {
			IPlugin* currPl = it->second->m_plugin;
			if (IsStringInList(currPl->GetName(),lowerPlugins)) {
				if (IsStringInList(plugin->GetName(),
					it->second->GetLowerDependencies())) {
					ret = SD_EINVALID_DEPENDENCY; /*ABBA*/
					goto __unlock_and_return;
				}
				altitudeLo = it->second->GetMyAltitude();
			}
		}
	}

	if (SD_ALTITUDE_INVALID == altitudeLo) {
		altitudeLo = SD_ALTITUDE_FIRST_IN_GROUP(newAltitude);
	}

	if (upperPlugins.length()>0) {
		for (PluginMapIt it = m_chain.begin(); it != m_chain.end(); ++it) {
			IPlugin* currPl = it->second->m_plugin;
			if (IsStringInList(currPl->GetName(),upperPlugins)) {
				if (IsStringInList(plugin->GetName(),
					it->second->GetUpperDependencies())) {
					ret = SD_EINVALID_DEPENDENCY; /*ABBA*/
					goto __unlock_and_return;
				}
				if (SD_ALTITUDE_INVALID == altitudeHi) {
					altitudeHi = it->second->GetMyAltitude();
				}
			}
		}
	}

	if (SD_ALTITUDE_INVALID == altitudeHi) {
		altitudeHi = SD_ALTITUDE_LAST_IN_GROUP(newAltitude);
	}

	if (altitudeHi <= altitudeLo) {
		/*
		 * The plugin has conflicting dependencies
		 */
		ret = SD_EINVALID_DEPENDENCY;
		goto __unlock_and_return;
	}

	if (newAltitude < altitudeLo || newAltitude > altitudeHi) {
		newAltitude = (altitudeLo+altitudeHi)/2;
	}

	while (m_chain.find(newAltitude) != m_chain.end())
	{
		assert(altitudeLo <= newAltitude && newAltitude <= altitudeHi);
		if (newAltitude - altitudeLo > altitudeHi - newAltitude) {
			altitudeHi = newAltitude;
		} else {
			altitudeLo = newAltitude;
		}
		newAltitude = (altitudeLo+altitudeHi)/2;
	}


	pluginContext = new PluginContext(plugin,this,
			newAltitude,lowerPlugins,upperPlugins);
	if (0 == pluginContext || !pluginContext->IsInitialized()) {
		ret = ENOMEM;
		goto __unlock_and_return;
	}
	pluginContext->AddRef();
	m_chain.insert(std::pair<SdPluginAltitude,PluginContext*>(
			newAltitude,pluginContext));

	printf("Attached plugin %s at altitude @%f \n",
			pluginContext->m_plugin->GetName(),
			pluginContext->GetMyAltitude());


	*pluginRuntime = pluginContext;
	ret = SD_ESUCCESS;

	__unlock_and_return:
	Unlock();

	if (pluginContext) {
		pluginContext->Release();
	}

	return ret;
}

void PluginChain::PluginDetach(
		PluginContext* pluginCtx)
{
	bool release = false;
	assert(pluginCtx);
	PluginMapIt it;
	LockWrite();
	it = m_chain.find(pluginCtx->GetMyAltitude());
	if (it != m_chain.end()) {
		m_chain.erase(it);
		release = true;
	}
	Unlock();
	if (release) {
		printf("Detached plugin %s from altitude @%f \n",
				pluginCtx->m_plugin->GetName(),
				pluginCtx->GetMyAltitude());
		pluginCtx->Release();
	}
}

bool PluginChain::IsStringInList(
	const char* str,
	const std::string& list)
{
	return list.find(str) != std::string::npos;
}

void PluginChain::MergeNameList(
		std::string* merged,
		const char* const list[])
{
	merged->clear();
	if (list) {
		for (int i = 0; i < 1024 && 0 != list[i]; i++) {
			merged->append(list[i]);
			merged->append("\n");
		}
	}
}

void PluginChain::LockWrite()
{
	pthread_rwlock_wrlock(&m_lock);
}

void PluginChain::Unlock()
{
	pthread_rwlock_unlock(&m_lock);
}

void PluginChain::LockRead()
{
	pthread_rwlock_rdlock(&m_lock);
}

void PluginChain::ReferencePluginList(
		PluginChain::RefedPluginListIterator* refList,
		PluginContext* caller,
		uint32_t dispatchFlags,
		SdDeviceId deviceFilter,
		uint32_t ioFilter)
{
	refList->Expand(m_chain.size());

	LockRead();
	PluginMapIt itBegin = m_chain.begin();
	PluginMapIt itEnd = m_chain.end();
	if (!(dispatchFlags & SD_FLAG_DISPATCH_TO_ALL)) {
		if (!caller) {
			goto __unlock; // caller must be specified
		}
		PluginMapIt callerIt = m_chain.find(caller->GetMyAltitude());
		if (callerIt == m_chain.end()) {
			goto __unlock; // plugin is getting removed ?
		}
		if (!(dispatchFlags & SD_FLAG_DISPATCH_DOWN)) {
			itBegin = ++callerIt;
		} else {
			itEnd = callerIt;
		}
	}
	refList->Expand(m_chain.size());
	refList->SetIterationDirection(!(dispatchFlags&SD_FLAG_DISPATCH_DOWN));
	for (PluginMapIt it = itBegin; it != itEnd; it++) {
		if ((it->second->GetPluginDeviceFilter() & deviceFilter) != deviceFilter) {
			continue;
		}
		if ((it->second->GetPluginIoCodeFilter() & ioFilter) != ioFilter) {
			continue;
		}
		if (!!(dispatchFlags & SD_FLAG_DISPATCH_SKIP_CALLER) &&
			it->second == caller) {
			continue;
		}
		refList->AddAndRefPlugin(it->second);
	}
	__unlock:
	Unlock();
}

int PluginChain::DispatchIo(
		PluginContext* caller,
		SdIoPacket* iop,
		uint32_t dispatchFlags)
{
	int err = SD_ESUCCESS;
	RefedPluginListIterator it;
	ReferencePluginList(
			&it,
			caller,
			dispatchFlags,
			SD_DEVICEID_TO_FLAG(iop->DeviceId()),
			SD_IOCODE_TO_FLAG(iop->IoCode()));
	for (it.BeginIterate(); 0 != it.Get() && SD_ESUCCESS == err; it.Next()) {
		err = it.Get()->m_plugin->IoCallback(iop);
	}
	if (SD_ESTOP_DISPATCH == err) {
		err = SD_ESUCCESS;
	}
	return err;
}

int PluginChain::StartPlugins(
		const _CommandArgs* cmdArgs)
{
	int err = SD_ESUCCESS;
	LockWrite();
	if (ArePluginsStarted()) {
		Unlock();
		return err;
	}
	for (PluginMapIt it = m_chain.begin(); it != m_chain.end(); it++) {
		printf("Starting plugin %s @%f \n",
			it->second->m_plugin->GetName(), it->second->GetMyAltitude());
		it->second->ConfigUpdated(cmdArgs);
		err = it->second->m_plugin->Start(cmdArgs);
		if (SD_ESUCCESS != err) {
			printf("Plugin %s failed to start, err=%d %s\n",
					it->second->m_plugin->GetName(), err, strerror(err));
			break;
		}
	}
	m_pluginsStarted = true;
	Unlock();
	if (0 != err) {
		StopPlugins(0);
	}
	return err;
}

int PluginChain::StopPlugins(bool detachPlugins)
{
	RefedPluginListIterator it;
	int stopFlag = detachPlugins ? IPlugin::FLAG_STOP_AND_DETACH : 0;
	ReferencePluginList(&it,0,
			SD_FLAG_DISPATCH_DOWN | SD_FLAG_DISPATCH_TO_ALL, 0, 0);
	for (it.BeginIterate(); 0 != it.Get(); it.Next()) {
		printf("Stopping plugin %s @%f \n",
				it.Get()->m_plugin->GetName(),
				it.Get()->GetMyAltitude());
		it.Get()->m_plugin->Stop(stopFlag);
	}
	m_pluginsStarted = false;
	return 0;
}

int PluginChain::ExecuteCommand(const _CommandArgs* cmdArgs)
{
	int err = 0;
	RefedPluginListIterator it;
	IoPacket* iop = new IoPacket(SD_ALTITUDE_FIRST,
			SD_IOCODE_COMMAND,
			SD_DEVICEID_COMMAND,
			"bcast");
	if (0 == iop) {
		return ENOMEM;
	}
	iop->SetIoData(SdIoData(cmdArgs),true);
	err = DispatchIo(0,iop,SD_FLAG_DISPATCH_TO_ALL);
	delete iop;
	return err;
}

PluginChain::RefedPluginListIterator::RefedPluginListIterator() :
		m_forwardIterate(true), m_index(0)
{
}

PluginChain::RefedPluginListIterator::~RefedPluginListIterator()
{
	for (size_t i = 0; i < m_plugins.size(); i++) {
			m_plugins.at(i)->Release();
		}
}

void PluginChain::RefedPluginListIterator::Expand(
		size_t size)
{
	m_plugins.reserve(size);
}

void PluginChain::RefedPluginListIterator::AddAndRefPlugin(
		PluginContext* plugin)
{
	plugin->AddRef();
	m_plugins.push_back(plugin);
}

void PluginChain::RefedPluginListIterator::BeginIterate()
{
	m_index = 0;
}

void PluginChain::RefedPluginListIterator::SetIterationDirection(
		bool forwardIterate)
{
	m_forwardIterate = forwardIterate;
}

PluginContext* PluginChain::RefedPluginListIterator::Get()
{
	PluginContext* context = 0;
	if (m_index < m_plugins.size()) {
		context = m_forwardIterate ? m_plugins.at(m_index) :
				m_plugins.at(m_plugins.size()-m_index-1);
	}
	return context;
}

void PluginChain::RefedPluginListIterator::Next()
{
	if (m_index < m_plugins.size()) {
		++m_index;
	}
}

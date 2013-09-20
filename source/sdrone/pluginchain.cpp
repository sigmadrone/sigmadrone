/*
 * pluginchain.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: svassilev
 */

#include "pluginchain.h"

PluginChain::PluginChain()
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
			RemovePluginContext(it->second);
		}
	}

	pthread_rwlock_destroy(&m_lock);
}

int PluginChain::PluginAttach(
	IPlugin* plugin,
	SdPluginAltitude _desiredAltitude,
	const char* const attachAbove[],
	const char* const attachBelow[],
	IPluginRuntime** pluginRuntime)
{
	int ret = EINVAL;
	std::string lowerPlugins;
	std::string upperPlugins;
	SdPluginAltitude newAltitude = SD_ALTITUDE_INVALID;
	SdPluginAltitude altitudeLo = SD_ALTITUDE_INVALID;
	SdPluginAltitude altitudeHi = SD_ALTITUDE_INVALID;
	SdPluginAltitude desiredAltitude = _desiredAltitude;
	PluginMapIt itLo = m_chain.end();
	PluginMapIt itHi = m_chain.end();
	PluginContext* pluginContext = 0;

	if (!plugin || !pluginRuntime) {
		return ret;
	}

	*pluginRuntime = 0;

	if (desiredAltitude >= SD_ALTITUDE_GROUP_LOWER_FILTER &&
		plugin->GetDeviceId() < SD_DEVICEID_FILTER) {
		desiredAltitude = SD_ALTITUDE_GROUP_DEVICE;
	}
	if (0 == attachAbove && 0 == attachBelow &&
		!(desiredAltitude >= SD_ALTITUDE_FIRST && desiredAltitude <= SD_ALTITUDE_LAST)) {
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
				itLo = it;
			}
		}
	}

	if (m_chain.end() == itLo) {
		itLo = FindNearestAltitude(desiredAltitude,true);
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
				if (m_chain.end() == itHi) {
					itHi = it;
				}
			}
		}
	}

	if (m_chain.end() == itHi) {
		itHi = FindNearestAltitude(desiredAltitude,false);
	}

	if (m_chain.end() != itHi && m_chain.end() != itLo &&
		itHi->second->GetMyAltitude() < itLo->second->GetMyAltitude()) {
		/*
		 * The plugin has conflicting dependencies
		 */
		ret = SD_EINVALID_DEPENDENCY;
		goto __unlock_and_return;
	}

	altitudeLo = SD_ALTITUDE_FIRST_IN_GROUP(desiredAltitude);
	if (m_chain.end() != itLo) {
		if (itLo->second->GetMyAltitude() > SD_ALTITUDE_LAST_IN_GROUP(desiredAltitude)) {
			ret = SD_EINVALID_DEPENDENCY;
			goto __unlock_and_return;
		}
		if (itLo->second->GetMyAltitude() <= SD_ALTITUDE_FIRST_IN_GROUP(desiredAltitude)) {
			altitudeLo = SD_ALTITUDE_FIRST_IN_GROUP(desiredAltitude);
		} else {
			altitudeLo = itLo->second->GetMyAltitude();
		}
	}

	altitudeHi = SD_ALTITUDE_LAST_IN_GROUP(desiredAltitude);
	if (m_chain.end() != itHi) {
		if (itHi->second->GetMyAltitude() < SD_ALTITUDE_FIRST_IN_GROUP(desiredAltitude)) {
			ret = SD_EINVALID_DEPENDENCY;
			goto __unlock_and_return;
		}
		if (itHi->second->GetMyAltitude() > SD_ALTITUDE_LAST_IN_GROUP(desiredAltitude)) {
			altitudeHi = SD_ALTITUDE_LAST_IN_GROUP(desiredAltitude);
		} else {
			altitudeHi = itHi->second->GetMyAltitude();
		}
	}

	if (altitudeLo == altitudeHi) {
		PluginMapIt it = m_chain.find(altitudeLo);
		assert(it != m_chain.end());
		altitudeHi = SD_ALTITUDE_LAST_IN_GROUP(altitudeHi);
		if (++it != m_chain.end()) {
			altitudeHi = fmin(altitudeHi,it->second->GetMyAltitude());
		}
		if (altitudeLo == altitudeHi) {
			assert(false);
			ret = SD_EINVALID_DEPENDENCY;
			goto __unlock_and_return;
		}
	}

	if (altitudeLo < desiredAltitude && desiredAltitude > altitudeHi) {
		newAltitude = desiredAltitude;
	} else {
		assert(altitudeLo < altitudeHi);
		newAltitude = (altitudeLo + altitudeHi)/2;
	}
	assert(m_chain.find(newAltitude) == m_chain.end());

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

void PluginChain::RemovePluginContext(
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
		pluginCtx->Release();
	}
}

PluginChain::PluginMapIt PluginChain::FindNearestAltitude(
		SdPluginAltitude altitude,
		bool low)
{
	for (PluginMapIt it = m_chain.begin(); it != m_chain.end(); it++) {
		if (low && it->second->GetMyAltitude() == altitude) {
			return it;
		}
		if (it->second->GetMyAltitude() > altitude) {
			return low ? --it : it;
		}
	}
	return m_chain.end();
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
	ReferencePluginList(&it,caller,dispatchFlags,iop->deviceId,iop->ioCode);
	for (it.BeginIterate(); 0 != it.Get() && SD_ESUCCESS == err; it.Next()) {
		err = it.Get()->m_plugin->IoCallback(iop);
	}
	if (SD_ESTOP_DISPATCH == err) {
		err = SD_ESUCCESS;
	}
	return err;
}

int PluginChain::StartPlugins(
		_CommandArgs* cmdArgs)
{
	int err = SD_ESUCCESS;
	LockWrite();
	for (PluginMapIt it = m_chain.begin(); it != m_chain.end(); it++) {
		printf("Starting plugin %s @%f \n",
			it->second->m_plugin->GetName(), it->second->GetMyAltitude());
		err = it->second->m_plugin->Start(cmdArgs);
		if (SD_ESUCCESS != err) {
			printf("Plugin %s failed to start, err=%d %s\n",
					it->second->m_plugin->GetName(), err, strerror(err));
			break;
		}
	}
	if (0 != err) {
		for (PluginMapIt it = m_chain.begin(); it != m_chain.end(); it++) {
			it->second->m_plugin->Stop(0);
		}
	}
	Unlock();
	return err;
}

int PluginChain::StopPlugins(bool detachPlugins)
{
	RefedPluginListIterator it;
	int stopFlag = detachPlugins ? IPlugin::FLAG_STOP_AND_DETACH : 0;
	ReferencePluginList(&it,0,
			SD_FLAG_DISPATCH_DOWN | SD_FLAG_DISPATCH_TO_ALL, 0, 0);
	for (it.BeginIterate(); 0 != it.Get(); it.Next()) {
		it.Get()->m_plugin->Stop(stopFlag);
	}
	return 0;
}

int PluginChain::ExecuteCommand(_CommandArgs* cmdArgs)
{
	int err = 0;
	RefedPluginListIterator it;
	SdIoPacket iop;
	SdIoPacket::Init(&iop,SD_IOCODE_COMMAND,SD_DEVICEID_ALL,"bcast");
	iop.inData.asCommandArgs = cmdArgs;
	iop.inData.dataType = SdIoData::TYPE_COMMAND_ARGS;
	ReferencePluginList(&it,0,SD_FLAG_DISPATCH_TO_ALL,0,0);
	for (it.BeginIterate(); 0 != it.Get() && SD_ESUCCESS == err; it.Next()) {
		it.Get()->m_plugin->IoCallback(&iop);
	}
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

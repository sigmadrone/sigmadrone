/*
 * pluginchain.h
 *
 *  Created on: Aug 15, 2013
 *      Author: svassilev
 */

#ifndef PLUGINCHAIN_H_
#define PLUGINCHAIN_H_

#include "commoninc.h"
#include <map>
#include <string>
#include <vector>
#include <pthread.h>
#include "plugincontext.h"
#include "commandargs.h"

class PluginChain {
public:
	PluginChain();
	~PluginChain();
	int PluginAttach(
		IPlugin* plugin,
		SdPluginAltitude desiredAltitude,
		const char* const attachAbove[],
		const char* const attachBelow[],
		IPluginRuntime** pluginRuntime
		);
	void RemovePluginContext(PluginContext*);
	int DispatchIo(
			PluginContext* issuer,
			SdIoPacket* iop,
			uint32_t dispatchFlags
			);

	int StartPlugins(_CommandArgs*);
	int StopPlugins(bool detachPlugins);
	int ExecuteCommand(_CommandArgs*);

private:
	typedef std::map<SdPluginAltitude,PluginContext*> PluginMap;
	typedef PluginMap::iterator PluginMapIt;

	class RefedPluginListIterator
	{
	public:
		RefedPluginListIterator();
		~RefedPluginListIterator();
		void SetIterationDirection(bool forwardIterate);
		void Expand(size_t size);
		void AddAndRefPlugin(PluginContext*);
		void BeginIterate();
		PluginContext* Get();
		void Next();
	private:
		std::vector<PluginContext*> m_plugins;
		bool m_forwardIterate;
		size_t m_index;
	};

	void ReferencePluginList(
			RefedPluginListIterator* refList,
			PluginContext* caller,
			uint32_t dispatchFlags
			);

	void LockWrite();
	void LockRead();
	void Unlock();
	PluginMapIt FindNearestAltitude(SdPluginAltitude alt,bool low);
	SdPluginAltitude GenerateAltitude(
			SdPluginAltitude prevLo,
			SdPluginAltitude desiredAltitude,
			SdPluginAltitude nextHi);

	static bool IsStringInList(
			const char* str,
			const std::string& list);

	static void MergeNameList(
			std::string* merged,
			const char* const list[]);
private:
	PluginMap m_chain;
	pthread_rwlock_t m_lock;
};

#endif /* PLUGINCHAIN_H_ */

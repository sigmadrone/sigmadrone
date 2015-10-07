/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
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
#include <boost/shared_ptr.hpp>

struct PluginChainIterator
{
	virtual ~PluginChainIterator() {}
	virtual void BeginIterate() = 0;
	virtual PluginContext* Get() = 0;
	virtual void Next() = 0;
};

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
	void PluginDetach(PluginContext*);
	int DispatchIo(
			PluginContext* issuer,
			SdIoPacket* iop,
			uint32_t dispatchFlags
			);
	int ExecuteCommand(
			SdCommandParams*,
			uint32_t dispatchFlags,
			const std::string& pluginName = std::string()
	        );
	PluginContext* RefPluginByName(const std::string& pluginName);
	bool IsPluginAttached(const std::string& pluginName);
	boost::shared_ptr<PluginChainIterator> RefPlugins();
private:
	typedef std::map<SdPluginAltitude,PluginContext*> PluginMap;
	typedef PluginMap::iterator PluginMapIt;

	class RefedPluginListIterator : public PluginChainIterator
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
			uint32_t dispatchFlags,
			SdDeviceId deviceFilter,
			uint32_t ioFilter
			);

	void LockWrite();
	void LockRead();
	void Unlock();
	PluginMapIt FindNearestAltitude(SdPluginAltitude alt,bool low);
	SdPluginAltitude GenerateAltitude(
			SdPluginAltitude prevLo,
			SdPluginAltitude desiredAltitude,
			SdPluginAltitude nextHi);
	int StopPlugins(bool detachPlugins);

	int ExecuteCommandForPlugin(SdCommandParams*, PluginContext*);

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

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

#ifndef PLUGINREGISTRY_H_
#define PLUGINREGISTRY_H_

#include "commoninc.h"
#include "jsonreadwrite.h"

struct PluginInfo {
	static const std::string state_load_pending;
	static const std::string state_unload_pending;
	static const std::string state_loaded;

	PluginInfo(const std::string& pluginName,
			const std::string& soName=std::string(),
			const std::string& state = state_load_pending,
			SdPluginAltitude altitude = 0.0) : plugin_name_(pluginName),
					so_name_(soName), state_(state), altitude_(altitude) {}
	~PluginInfo() {}
	const std::string& Name() const { return plugin_name_; }
	const std::string& SoName() const { return so_name_; }
	bool IsLoadPending() const { return state_ == state_load_pending; }
	bool IsUnloadPending() const { return state_ == state_unload_pending; }
	void UnloadPending() { state_ = state_unload_pending; }
	const std::string& State() const { return state_; }
	SdPluginAltitude Attitude() const { return altitude_; }
private:
	std::string plugin_name_;
	std::string so_name_;
	std::string state_;
	SdPluginAltitude altitude_;
};

struct PluginRegistry {
	PluginRegistry();
	~PluginRegistry();
	void AddPlugin(const PluginInfo&);
	void RemovePlugin(const std::string& pluginName);
	void PluginUnloadPending(const std::string& pluginName);
	void CleanupUnloadedPlugins();
	size_t Count();
	const PluginInfo& Plugin(size_t index);
	void Reset() { plugins_.clear(); }
private:
	std::vector<PluginInfo> plugins_;
};

#endif /* PLUGINREGISTRY_H_ */

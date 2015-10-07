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

#include "pluginregistry.h"

const std::string PluginInfo::state_load_pending = "LoadPending";
const std::string PluginInfo::state_unload_pending = "UnloadPending";
const std::string PluginInfo::state_loaded = "Loaded";

PluginRegistry::PluginRegistry() {}

PluginRegistry::~PluginRegistry() {}

void PluginRegistry::AddPlugin(const PluginInfo& plugin) {
	RemovePlugin(plugin.Name());
	plugins_.push_back(plugin);
}

void PluginRegistry::RemovePlugin(const std::string& pluginName) {
	for (size_t i = 0; i < Count(); i++) {
		if (plugins_[i].Name() == pluginName) {
			plugins_.erase(plugins_.begin()+i);
			break;
		}
	}
}

size_t PluginRegistry::Count() {
	return plugins_.size();
}

const PluginInfo& PluginRegistry::Plugin(size_t index) {
	assert(index < Count());
	return plugins_[index];
}

void PluginRegistry::PluginUnloadPending(
		const std::string& pluginName)
{
	for (size_t i = 0; i < Count(); i++) {
		if (plugins_[i].Name() == pluginName) {
			plugins_[i].UnloadPending();
			break;
		}
	}
}

void PluginRegistry::CleanupUnloadedPlugins()
{
	for (size_t i = 0; i < Count(); ) {
		if (plugins_[i].State() != PluginInfo::state_loaded) {
			plugins_.erase(plugins_.begin()+i);
		} else {
			++i;
		}
	}
}

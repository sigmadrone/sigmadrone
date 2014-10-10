/*
 * pluginregistry.cpp
 *
 *  Created on: October 7, 2014
 *      Author: svassilev
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

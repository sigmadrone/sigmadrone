/*
 * pluginregistry.cpp
 *
 *  Created on: October 7, 2014
 *      Author: svassilev
 */

#include "pluginregistry.h"

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

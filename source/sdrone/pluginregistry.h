/*
 * pluginregistry.h
 *
 *  Created on: October 7, 2014
 *      Author: svassilev
 */

#ifndef PLUGINREGISTRY_H_
#define PLUGINREGISTRY_H_

#include "commoninc.h"
#include "jsonreadwrite.h"

struct PluginInfo {
	PluginInfo(const std::string& pluginName,
			const std::string& soName=std::string(),
			bool load=true) : plugin_name_(pluginName), so_name_(soName), load_(load) {}
	~PluginInfo() {}
	const std::string& Name() const { return plugin_name_; }
	const std::string& SoName() const { return so_name_; }
	bool Load() const { return load_; }
private:
	std::string plugin_name_;
	std::string so_name_;
	bool load_;
};

struct PluginRegistry {
	PluginRegistry();
	~PluginRegistry();
	void AddPlugin(const PluginInfo&);
	void RemovePlugin(const std::string& pluginName);
	size_t Count();
	const PluginInfo& Plugin(size_t index);
private:
	std::vector<PluginInfo> plugins_;
};

#endif /* PLUGINREGISTRY_H_ */

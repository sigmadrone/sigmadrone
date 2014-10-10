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

/*
 * plugincommandparams.h
 *
 *  Created on: Aug 26, 2014
 *      Author: svassilev
 */
#ifndef PLUGINCOMMANDPARAMS_H_
#define PLUGINCOMMANDPARAMS_H_


struct PluginCommandParams: public SdCommandParams
{
	PluginCommandParams(SdCommandCode cmdCode, const SdIoData& data,
			const IJsonValue* rpcParams) : m_cmdCode(cmdCode),
					m_data(data), m_rpcParams(rpcParams) {}
	PluginCommandParams(SdCommandCode cmdCode) : m_cmdCode(cmdCode),
			m_data(nullSdIoData), m_rpcParams(0) {}
	~PluginCommandParams() {}
	virtual SdCommandCode CommandCode() { return m_cmdCode; }
	virtual const SdIoData& Params() { return m_data; }
	virtual const IJsonValue* RpcParams() { return m_rpcParams; }

private:
	SdCommandCode m_cmdCode;
	const SdIoData& m_data;
	const IJsonValue* m_rpcParams;

	static const SdIoData nullSdIoData;
};

const SdIoData PluginCommandParams::nullSdIoData __attribute__((weak));

#endif /* PLUGINCOMMANDPARAMS_H_ */

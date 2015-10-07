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
#ifndef PLUGINCOMMANDPARAMS_H_
#define PLUGINCOMMANDPARAMS_H_


struct PluginCommandParams: public SdCommandParams
{
	PluginCommandParams(SdCommandCode cmdCode, const SdIoData& data,
			const IJsonValue* rpcParams) : m_cmdCode(cmdCode),
					m_inParams(data), m_rpcParams(rpcParams) {}
	PluginCommandParams(SdCommandCode cmdCode, const SdIoData& inData,
			const IJsonValue* rpcParams, const SdIoData& outData) : m_cmdCode(cmdCode),
				m_inParams(inData), m_outParams(outData), m_rpcParams(rpcParams) {}
	PluginCommandParams(SdCommandCode cmdCode) : m_cmdCode(cmdCode),
			m_inParams(nullSdIoData), m_rpcParams(0) {}
	~PluginCommandParams() {}
	virtual SdCommandCode CommandCode() { return m_cmdCode; }
	virtual const SdIoData& Params() { return m_inParams; }
	virtual const IJsonValue* RpcParams() { return m_rpcParams; }
	virtual void SetOutParams(const SdIoData& outParams) {
		m_outParams = outParams;
	}
	virtual const SdIoData& OutParams() { return m_outParams; }

private:
	SdCommandCode m_cmdCode;
	SdIoData m_inParams;
	SdIoData m_outParams;
	const IJsonValue* m_rpcParams;

	static const SdIoData nullSdIoData;
};

const SdIoData PluginCommandParams::nullSdIoData __attribute__((weak));

#endif /* PLUGINCOMMANDPARAMS_H_ */

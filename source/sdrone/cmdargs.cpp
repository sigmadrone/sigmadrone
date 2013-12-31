#include <sstream>
#include <stdexcept>
#include "cmdargs.h"
#include <stdlib.h>

const std::string CmdArgs::s_emptyValue = "";

CmdArgs::CmdArgs(
		const std::string &argprefix,
		const std::string &aliasprefix,
		unsigned int helpwidth,
		unsigned int aliaswidth
		) :
		m_argprefix(argprefix),
		m_aliasprefix(aliasprefix),
		m_helpwidth(helpwidth),
		m_aliaswidth(aliaswidth)
{

}

CmdArgs::~CmdArgs()
{

}


static std::pair<std::string, std::string> GetNameValue(const std::string &str)
{
	std::string name, value;
	unsigned int i = 0;

	if (!str.length())
		goto end;
	if (str.length() == 0 || !::isalpha(str[i]))
		goto end;
	for (i = 0; i < str.length(); i++) {
		if (::isblank(str[i])) {
			goto end;
		} else if (str[i] == '=') {
			++i;
			value = str.substr(i);
			break;
		} else {
			name = str.substr(0, i + 1);
		}

	}
end:
	return std::pair<std::string, std::string>(name, value);
}

void CmdArgs::AddSpecs(CmdArgSpec *specs, unsigned int count)
{
	while (count--) {
		m_specs.push_back(*specs);
		++specs;
	}
}


std::string CmdArgs::HelpMessage()
{
	std::stringstream oss;
	std::vector<CmdArgSpec>::const_iterator it;

	for (it = m_specs.begin(); it != m_specs.end(); it++) {
		std::stringstream temp, alias;
		oss << "    ";
		oss.width(m_aliaswidth);
		oss.fill(' ');
		if (!it->alias.empty())
			alias << m_aliasprefix << it->alias << ",";
		oss << std::left << alias.str();
		switch (it->type) {
		case CMD_ARG_STRING:
			temp << it->name << "=<arg>";
			break;
		case CMD_ARG_MULTISTRING:
			temp << it->name << "=<arg>";
			temp << " [" << m_argprefix << it->name << "=<arg>" << " ...]";
			break;
		default:
		case CMD_ARG_BOOL:
			temp << it->name;
			break;
		}
		oss << m_argprefix;
		oss.width(m_helpwidth);
		oss.fill(' ');
		oss << std::left << temp.str();
		oss << " ";
		oss << it->help;
		oss << std::endl;
	}
	return oss.str();
}

void CmdArgs::Parse(int argc, char *argv[]) throw(std::exception)
{
	std::vector<std::string> empty;
	std::vector<CmdArgSpec>::const_iterator itspec;

	for (int i = 1; i < argc; i++) {
		std::map<std::string, std::vector<std::string> >::iterator it;
		std::string argname = argv[i];
		std::pair<std::string, std::string> nameval;

		if (argname.length() > m_argprefix.length() && argname.substr(0, m_argprefix.length()) == m_argprefix) {
			nameval = GetNameValue(argname.substr(m_argprefix.length()));
			for (itspec = m_specs.begin(); itspec != m_specs.end(); itspec++) {
				if (itspec->name == nameval.first)
					break;
			}
			if (itspec->type != CMD_ARG_BOOL && nameval.second.empty()) {
				std::stringstream oss;
				oss << "Invalid value for: " << nameval.first;
				throw(std::out_of_range(oss.str()));
			}
		} else if (argname.length() > m_aliasprefix.length() && argname.substr(0, m_aliasprefix.length()) == m_aliasprefix) {
			nameval = GetNameValue(argname.substr(m_aliasprefix.length()));
			for (itspec = m_specs.begin(); itspec != m_specs.end(); itspec++) {
				if (itspec->alias == nameval.first) {
					nameval.first = itspec->name;
					break;
				}
			}
		} else {
			continue;
		}
		if (itspec == m_specs.end() ||
			(itspec->type == CMD_ARG_BOOL && !nameval.second.empty())) {
			std::stringstream oss;
			oss << "Invalid argument: " << nameval.first;
			throw(std::out_of_range(oss.str()));
		}
		it = m_args.insert(std::pair<std::string, std::vector<std::string> >(nameval.first, empty)).first;
		if (itspec->type != CMD_ARG_MULTISTRING)
			it->second.clear();
		if (itspec->type == CMD_ARG_BOOL)
			it->second.push_back("1");
		else
			it->second.push_back(nameval.second);
	}
}


const std::string& CmdArgs::GetValue(const std::string &name)
{
	std::map<std::string, std::vector<std::string> >::const_iterator it;
	it = m_args.find(name);
	if (it != m_args.end()) {
		return *it->second.begin();
	}

	return s_emptyValue;
}

bool CmdArgs::GetValueAsDouble(const std::string &name, double* value)
{
	std::string strVal = GetValue(name);
	if (strVal.length()) {
		*value = atof(strVal.c_str());
	}
	return !!strVal.length();
}

bool CmdArgs::GetValueAsInt(const std::string &name, int32_t* value)
{
	std::string strVal = GetValue(name);
	if (strVal.length()) {
		*value = atoi(strVal.c_str());
	}
	return !!strVal.length();
}

bool CmdArgs::GetBoolValue(const std::string &name)
{
	return GetValue(name) == "1";
}

std::vector<std::string> CmdArgs::GetValues(const std::string &name)
{
	std::vector<std::string> ret;
	std::map<std::string, std::vector<std::string> >::const_iterator it;

	it = m_args.find(name);
	if (it != m_args.end()) {
		ret = it->second;
	}
	return ret;
}

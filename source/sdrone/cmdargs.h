#ifndef _CMDARGS_H_
#define _CMDARGS_H_

#include <vector>
#include <map>
#include <string>
#include <exception>
#include <sys/types.h>
using namespace std;

enum CmdArgType {
	CMD_ARG_BOOL = 0,
	CMD_ARG_STRING,
	CMD_ARG_MULTISTRING,
};

struct CmdArgSpec
{
	std::string name;
	std::string alias;
	std::string help;
	CmdArgType type;
};


class CmdArgs
{
public:
	CmdArgs(const std::string &argprefix = "--",
			const std::string &aliasprefix = "-",
			unsigned int helpwidth = 30,
			unsigned int aliaswidth = 5);
	~CmdArgs();
	void AddSpecs(CmdArgSpec *specs, unsigned int count);
	void Parse(int argc, char *argv[]) throw (std::exception);
	const std::string& GetValue(const std::string &name);
	std::vector<std::string> GetValues(const std::string &name);
	std::string HelpMessage() const;

	bool /*value present*/ GetValueAsDouble(const std::string &name, double* value);
	bool /*value present*/ GetValueAsInt(const std::string &name, int32_t* value);
	bool /*value*/ GetBoolValue(const std::string &name);

public:
	std::map<std::string, std::vector<std::string> > m_args;

private:
	std::vector<CmdArgSpec> m_specs;
	std::string m_argprefix;
	std::string m_aliasprefix;
	unsigned int m_helpwidth;
	unsigned int m_aliaswidth;

	static const std::string s_emptyValue;
};


#endif


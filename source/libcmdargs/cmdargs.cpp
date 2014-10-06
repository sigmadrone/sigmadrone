#include <sstream>
#include <stdexcept>
#include "cmdargs.h"

cmd_args::cmd_args(
		const std::string &argprefix,
		const std::string &aliasprefix,
		unsigned int helpwidth,
		unsigned int aliaswidth
		) :
		argprefix_(argprefix),
		aliasprefix_(aliasprefix),
		helpwidth_(helpwidth),
		aliaswidth_(aliaswidth)
{

}

cmd_args::~cmd_args()
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

void cmd_args::add_specs(cmd_arg_spec *specs, unsigned int count)
{
	while (count--) {
		specs_.push_back(*specs);
		++specs;
	}
}


std::string cmd_args::get_help_message() const
{
	std::stringstream oss;
	std::vector<cmd_arg_spec>::const_iterator it;

	for (it = specs_.begin(); it != specs_.end(); it++) {
		std::stringstream temp, alias;
		oss << "    ";
		oss.width(aliaswidth_);
		oss.fill(' ');
		if (!it->alias.empty())
			alias << aliasprefix_ << it->alias << ",";
		oss << std::left << alias.str();
		switch (it->type) {
		case CMD_ARG_STRING:
			temp << it->name << "=<arg>";
			break;
		case CMD_ARG_MULTISTRING:
			temp << it->name << "=<arg>";
			temp << " [" << argprefix_ << it->name << "=<arg>" << " ...]";
			break;
		default:
		case CMD_ARG_BOOL:
			temp << it->name;
			break;
		}
		oss << argprefix_;
		oss.width(helpwidth_);
		oss.fill(' ');
		oss << std::left << temp.str();
		oss << " ";
		oss << it->help;
		oss << std::endl;
	}
	return oss.str();
}

void cmd_args::parse_command_line(int argc, const char *argv[]) throw(std::exception)
{
	std::vector<std::string> empty;
	std::vector<cmd_arg_spec>::const_iterator itspec;

	for (int i = 1; i < argc; i++) {
		std::map<std::string, std::vector<std::string> >::iterator it;
		std::string argname = argv[i];
		std::pair<std::string, std::string> nameval;

		if (argname.length() > argprefix_.length() && argname.substr(0, argprefix_.length()) == argprefix_) {
			nameval = GetNameValue(argname.substr(argprefix_.length()));
			for (itspec = specs_.begin(); itspec != specs_.end(); itspec++) {
				if (itspec->name == nameval.first)
					break;
			}
		} else if (argname.length() > aliasprefix_.length() && argname.substr(0, aliasprefix_.length()) == aliasprefix_) {
			nameval = GetNameValue(argname.substr(aliasprefix_.length()));
			for (itspec = specs_.begin(); itspec != specs_.end(); itspec++) {
				if (itspec->alias == nameval.first) {
					nameval.first = itspec->name;
					break;
				}
			}
		} else {
			continue;
		}
		if (itspec == specs_.end() ||
			(itspec->type == CMD_ARG_BOOL && !nameval.second.empty())) {
			std::stringstream oss;
			oss << "Invalid command line argument: " << nameval.first;
			throw(std::out_of_range(oss.str()));
		}
		it = args_.insert(std::pair<std::string, std::vector<std::string> >(nameval.first, empty)).first;
		if (itspec->type != CMD_ARG_MULTISTRING)
			it->second.clear();
		if (itspec->type == CMD_ARG_BOOL)
			it->second.push_back("1");
		else
			it->second.push_back(nameval.second);
	}
}


std::string cmd_args::get_value(const std::string &name, const std::string& defval) const
{
	std::string ret = defval;
	std::map<std::string, std::vector<std::string> >::const_iterator it;

	it = args_.find(name);
	if (it != args_.end()) {
		ret = *it->second.begin();
	}

	return ret;
}

std::vector<std::string> cmd_args::get_values(const std::string &name) const
{
	std::vector<std::string> ret;
	std::map<std::string, std::vector<std::string> >::const_iterator it;

	it = args_.find(name);
	if (it != args_.end()) {
		ret = it->second;
	}
	return ret;
}

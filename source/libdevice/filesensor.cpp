#include <fstream>
#include <cstdlib>
#include "filesensor.h"

file_sensor::file_sensor(const std::string& filename, unsigned int trottle, double scale)
	: filename_(filename)
	, trottle_(trottle)
	, trottle_counter_(0)
	, cachedvalue_(0)
	, scale_(scale)
{

}

file_sensor::~file_sensor()
{
}

void file_sensor::open()
{
	std::ifstream file;
	file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	file.open(filename_.c_str());
	file.close();
}

double file_sensor::read()
{
	if (trottle_ && (trottle_counter_++ % trottle_)) {
		return cachedvalue_;
	}
	std::string strval;
	std::ifstream file;
	file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	file.open(filename_.c_str());
	file >> strval;
	file.close();
	cachedvalue_ = strtod(strval.c_str(), NULL) * scale_;
	return cachedvalue_;
}

void file_sensor::read(double &value)
{
	value = read();
}

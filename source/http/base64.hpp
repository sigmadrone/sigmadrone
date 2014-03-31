#ifndef HTTP_BASE64_HPP
#define HTTP_BASE64_HPP

#include <string>
#include <boost/system/error_code.hpp>

namespace http {
namespace base64 {

std::string encode(const std::string& s);
std::string decode(const std::string& s, boost::system::error_code& ec);
std::string decode(const std::string& s) throw (std::exception);

} // namespace base64
} // namespace http

#endif

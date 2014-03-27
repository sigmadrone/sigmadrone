#ifndef HTTP_BASE64_HPP
#define HTTP_BASE64_HPP

#include <string>

namespace http {
namespace base64 {

std::string encode(const std::string& s);
std::string decode(const std::string& s);
const char* base64_decode(char* dest, const char* src, unsigned int len);

} // namespace base64
} // namespace http

#endif

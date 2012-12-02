#ifndef __KXUTIL3_UTF8_CONVERT_HPP__
#define __KXUTIL3_UTF8_CONVERT_HPP__

#include<string>
using std::string;
namespace kxutil4 { 
namespace Encoding{

int IconvWrapper(char *out, size_t out_length, const char *in, size_t in_length, 
                  const char *from_charset, const char *to_charset);

string IconvWrapper(const string &source, const char *from_charset, const char *to_charset);

string UTF82GBK(const string &utf8_buffer);

string GBK2UTF8(const string &gbk_buffer);

} // namespace Encoding
} // namespace kxutil4
#endif

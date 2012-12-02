#include "utf8_conv.hpp"
#include <iconv.h>
#include <errno.h>
#include <assert.h>
#include <string>
#include <iostream>

using namespace std;
namespace kxutil4 {
namespace Encoding{

int IconvWrapper(char *out, size_t out_length, const char *in, size_t in_length, 
        const char *to_charset, const char *from_charset) {
    assert(out != NULL || in != NULL);
    bool flag = false;

    iconv_t cd = iconv_open(to_charset, from_charset);
    if (cd != (iconv_t)-1) {
        char* pin = (char*)in;
        size_t in_left = in_length;

        size_t out_left = out_length - 1;
        char* pout = out;
        size_t iconv_ret ;
        while(in_left > 0){
            //cout << "1 in_left: " << in_left << ", out_left: " << out_left << endl;
            iconv_ret = iconv(cd, &pin, &in_left, &pout, &out_left);
            //cout << "2 in_left: " << in_left << ", out_left: " << out_left << endl;
            if(iconv_ret == (size_t)-1){
                //printf("%s to %s\n", in, to_charset);
                pin++;
                in_left--;
                flag = true;
                //printf("IconvWrapper: errno %d\n", errno);
                if (errno == E2BIG)
                    break;
            }
        }
        iconv_close(cd);
        out[out_length -1 - out_left] = '\0';
    }
    else {
        return -1; // failed
    }
    if (flag)
        return -2; // succeed but have error
    return 0; // suceed
}

string IconvWrapper(const string &source, const char *to_charset, const char *from_charset) {
    size_t buf_len = source.size()*6 + 1;
	char *buf = new char[buf_len];
	if (buf==NULL)
		return "";

    int ret = IconvWrapper(buf, buf_len, source.c_str(), source.size(), to_charset, from_charset);
    if (ret < 0)
        return "";
    string tmp = buf;
	delete[] buf;
	return tmp;
}

string UTF82GBK(const string &utf8_buffer) {
    return IconvWrapper(utf8_buffer, "gbk", "utf8");
}

string GBK2UTF8(const string &gbk_buffer) {
    return IconvWrapper(gbk_buffer, "utf8", "gbk");
}

} // namespace Encoding
} // namespace kxutil4

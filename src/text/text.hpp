#ifndef __KXUTIL3_TEXT_HPP__
#define __KXUTIL3_TEXT_HPP__
#include <string>
#include <vector>
#include <sstream>

using namespace std;

namespace kxutil4 {
namespace Text {

size_t FindGBK(const string &source, const string &seperator, size_t start = 0);

void Segment(const string &source, const string &seperator, vector<string> &pieces);

void SegmentGBK(const string &source, const string &seperator, vector<string> &pieces);

void Segment2(const string &source, const string &seperator, vector<string> &pieces, bool s_flag=false);

string Join(const string seperator, vector<string> &items);

string Int2String(int i);

string Long2String(long l);

template<class T>
string ToString(const T &in) {
    stringstream oss;
    oss << in;
    string temp;
    oss >> temp;
    return temp ;
}

string ToLower(const string &src);

string ToUpper(const string &src);

string ToLowerGBK(const string &str);

string ToUpperGBK(const string &str);

bool IsNumber(const string &str);

string LTrim(const string &source, const string &seperators = " \t\n\r");

string RTrim(const string &source, const string &seperators = " \t\n\r");

string Trim(const string &source, const string &seperators = " \t\n\r");

void StringReplace(string &content, const string &from, const string &to, bool full_flag=true);

void StringReplaceGBK(string &content, const string &from, const string &to);

int Count(const string &source, const string &sub);

bool IsEndsWith(const string &source, const string &sub);

} // namespace Text
} // namespace kxutil4
#endif // __KXUTIL3_TEXT_HPP__

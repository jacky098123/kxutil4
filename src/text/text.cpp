#include "text.hpp"

#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <vector>
#include <iostream>

using namespace std;
namespace kxutil4 {
namespace Text {

// it works like the stl string.find(), beside gbk charset
// it is something like old SecureStringFind function
size_t FindGBK(const string &source, const string &seperator, size_t start) {
    // supposing the source and seperator is ok, no half multi-char
    size_t  source_length       = source.size();
    size_t  seperator_length    = seperator.size();
    const unsigned char *psource    = (const unsigned char*)source.c_str();
    const unsigned char *pseperator = (const unsigned char*)seperator.c_str();
    if (start >= source_length || source_length < seperator_length)
        return string::npos;

    for (size_t i=start; i<=source_length - seperator_length; ) {
        size_t j;
        for (j=0; j<seperator_length; j++) {
            if (psource[i+j] == pseperator[j])
                continue;
            else
                break;
        } // for
        if (j < seperator_length) {
            // step forward
            if (psource[i] <= 127)
                i++;
            else
                i += 2;
        } else if (j == seperator_length) {
            // matched
            return i;
        } else {
            return string::npos;
        }
    }
    return string::npos;
}

map<string,string> String2Map(const string &str, const string &sep1, const string &sep2) {
    vector<string> list;
    Segment(str, sep1, list);

    map<string,string> tmp;
    for (size_t i=0; i<list.size(); i++) {
        vector<string> item;
        Segment(list[i], sep2, item);
        if (item.size() != 2)
            continue;

        tmp.insert(map<string,string>::value_type(item[0], item[1]));
    }
    return tmp;
}

void Segment(const string &source, const string &seperator, vector<string> &pieces) {
    size_t pos;
    size_t start = 0;

    pieces.clear();
    do {
        pos = source.find(seperator, start);
        if (pos == string::npos) {
            if (start < source.size()) {
                pieces.push_back(source.substr(start));
            } else if (start == source.size()) {
                pieces.push_back("");
            }
        } else {
            if (start < pos) {
                pieces.push_back(source.substr(start, pos - start));
            } else { // add empty string
                pieces.push_back("");
            }
        }
        start = pos + seperator.size();
    } while (pos != string::npos);
}

// the same as Segment, beside can handle gbk charset
void SegmentGBK(const string &source, const string &seperator, vector<string> &pieces) {
    size_t pos;
    size_t start = 0;

    pieces.clear();
    do {
        pos = FindGBK(source, seperator, start);
        if (pos == string::npos) {
            if (start < source.size()) {
                pieces.push_back(source.substr(start));
            } else if (start == source.size()) {
                pieces.push_back("");
            }
        } else {
            if (start < pos) {
                pieces.push_back(source.substr(start, pos - start));
            } else { // add empty string
                pieces.push_back("");
            }
        }
        start = pos + seperator.size();
    } while (pos != string::npos);
}

void Segment2(const string &source, const string &seperator, vector<string> &pieces, bool s_flag) {
    size_t pos;
    size_t start = 0;

    pieces.clear();
    do {
        pos = source.find(seperator, start);
        if (pos == string::npos) {
            if (start < source.size()) {
                pieces.push_back(source.substr(start));
            } else if (start == source.size()) {
                if (!s_flag) {
                    pieces.push_back("");
                }
            }
        } else {
            if (start < pos) {
                pieces.push_back(source.substr(start, pos - start));
            } else { // add empty string
                if (!s_flag) {
                    pieces.push_back("");
                }
            }
        }
        start = pos + seperator.size();
    } while (pos != string::npos);
}

string Join(const string seperator, vector<string> &items) {
    string destination = "";

    if (items.size() == 0)
        return destination;

    destination = items[0];
    for (size_t i=1; i<items.size(); i++) {
        destination += seperator + items[i];
    }
    return destination;
}

string Int2String(int i) {
    char buffer[32];

    snprintf(buffer, sizeof(buffer), "%d", i);
    return buffer;
}

string Long2String(long l) {
    char buffer[32];

    snprintf(buffer, sizeof(buffer), "%ld", l);
    return buffer;
}

string ToLower(const string &src) {
    string dest = src;
    size_t offset = 'a' - 'A';

    for (size_t i=0; i<dest.size(); i++) {
        if (dest[i] >= 'A' && dest[i] <= 'Z')
            dest[i] = dest[i] + offset;
    }
    return dest;
}

string ToUpper(const string &src) {
    string dest = src;
    size_t offset = 'a' - 'A';

    for (size_t i=0; i<dest.size(); i++) {
        if (dest[i] >= 'a' && dest[i] <= 'z')
            dest[i] = dest[i] - offset;
    }
    return dest;
}

string ToLowerGBK(const string &str) {
    size_t step = 1;
    string return_string = str;
    for (size_t i=0;i<return_string.length();){
        step = 1;
        size_t ti = i;
        if( (return_string[ti]<0) ){
            step = 2;
            if(ti< (return_string.length()-1) ){
                if( (return_string[ti+1]<64) && (return_string[ti+1]>0)){
                    step =1 ;
                }
            }
        }else{
            if ( return_string[ti]>='A' && return_string[ti]<='Z'){
                return_string[ti]=return_string[ti]-'A'+'a';
            }
        }
        i+= step ;
    }
    return return_string;
}

string ToUpperGBK(const string &str) {
    size_t step = 1;
    string return_string = str;
    for (size_t i=0;i<return_string.length();){
        step = 1;
        size_t ti = i;
        if( (return_string[ti]<0) ){
            step = 2;
            if(ti< (return_string.length()-1) ){
                if( (return_string[ti+1]<64) && (return_string[ti+1]>0)){
                    step =1 ;
                }
            }
        }else{
            if ( return_string[ti]>='a' && return_string[ti]<='z'){
                return_string[ti]=return_string[ti]-'a'+'A';
            }
        }
        i+= step ;
    }
    return return_string;
}

string LTrim(const string &source, const string &seperators) {
    if(source.empty()) {
        return "";
    }
    size_t position = 0;
    while(position < source.length()) {
        char ch = source[position];
        if( seperators.find(ch) == string::npos) {
            break;
        }
        position++;
    }
    return source.substr(position);
}

bool IsNumber(const string &str) {
    int low  = (int)'0';
    int high = (int)'9';
    for (size_t i=0; i<str.size(); i++) {
        unsigned char c = (unsigned char)str[i];
        if (c < low || c > high) {
            return false;
        }
    }
    return true;
}


string RTrim(const string &source, const string &seperators) {
    if(source.empty()) {
        return "";
    }
    size_t position = source.length()-1;
    while(position > 0){
        char ch = source[position];
        if( seperators.find(ch)==string::npos) {
            break;
        }
        position--;
    }
    if(position == 0) {
        if(seperators.find(source[position]) != string::npos) {
            return "";
        }
    }
    return source.substr(0, position+1);
}

string Trim(const string &source, const string &seperators) {
    return RTrim(LTrim(source, seperators), seperators);
}

void StringReplace(string &content, const string &from, const string &to, bool full_flag) {
    string::size_type   pos=0;
    string::size_type   srclen=from.size();
    string::size_type   dstlen=to.size();
    bool                while_flag = true;

    while((pos=content.find(from, pos)) != string::npos && while_flag) {
        content.replace(pos, srclen, to);
        pos += dstlen;
        if (!full_flag)
            while_flag = false;
    }
}

void StringReplaceGBK(string &content, const string &from, const string &to) {
    string::size_type pos=0;
    string::size_type srclen=from.size();
    string::size_type dstlen=to.size();
    while((pos=FindGBK(content, from, pos)) != string::npos) {
        content.replace(pos, srclen, to);
        pos += dstlen;
    }
}

int Count(const string &source, const string &sub) {
    string::size_type pos=0;
    int     count = 0;

    while ((pos = source.find(sub, pos)) != string::npos) {
        pos += sub.size();
        count++;
    }
    return count;
}

bool IsEndsWith(const string &source, const string &sub) {
    if (source.size() < sub.size())
        return false;
    string tmp = source.substr(source.size() - sub.size());
    if (tmp == sub)
        return true;
    return false;
}

} // namespace File
} // namespace kxutil4

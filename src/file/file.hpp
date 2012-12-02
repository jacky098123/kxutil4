#ifndef __KXUTIL3_FILE_HPP__
#define __KXUTIL3_FILE_HPP__

#include <stdio.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

namespace kxutil4 {
namespace File {

string LoadFile(const string &full_file_name);

bool SaveFile(const string full_file_name, const string& content);

bool AppendFile(const string full_file_name, const string& content);

bool LoadListFromFile(const string &full_file_name, vector<string> &result);

bool SaveListToFile(const string &full_file_name, vector<string> &list);

bool LoadMapFromFile(const string &full_file_name, map<string, string> &result, 
                     const string &seperator="=");

void Trim(string &str, const string &seperators=" \t\r\n");

size_t ScanDir(const string &path, vector<string> &file_list, bool append_path_flag = false);
} // namespace File
} // namespace kxutil4
#endif // __KXUTIL3_FILE_HPP__

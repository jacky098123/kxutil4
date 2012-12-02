#include "file.hpp"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include <string>
#include <vector>
#include <fstream>

using namespace std;
namespace kxutil4 {
namespace File {
string LoadFile(const string &full_file_name) {
    //printf("LoadFile %s\n", full_file_name.c_str());
    FILE* f = fopen(full_file_name.c_str(), "rb");
    if(f == NULL) {
        printf("LoadFile %s fail\n", full_file_name.c_str());
        return "";
    }
    fseek(f, 0, SEEK_END);
    size_t flen = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = new char[flen + 1];
    flen = fread(buf, 1, flen, f);
    buf[flen] = '\0';
    fclose(f);
    string result = string(buf, flen);
    delete[] buf;
    return result;
}

bool SaveFile(const string full_file_name, const string& content) {
    FILE* fp = fopen(full_file_name.c_str(), "wb");
    if(fp == NULL)
        return false;
    fwrite(content.c_str(), 1, content.length(), fp);
    fclose(fp);
    return true;
}

bool AppendFile(const string full_file_name, const string& content) {
    FILE* fp = fopen(full_file_name.c_str(), "a");
    if(fp == NULL)
        return false;
    fwrite(content.c_str(), 1, content.length(), fp);
    fclose(fp);
    return true;
}

bool LoadListFromFile(const string &full_file_name, vector<string> &result) {
    string content; 

    result.clear();
    if (access(full_file_name.c_str(), R_OK) < 0)
        return false;
    content = LoadFile(full_file_name);
    if (content.size() > 0) {
        size_t pos1=0, pos2=0;
        string sub_str;
        while (1) {
            pos2 = content.find("\n", pos1);
            if (pos2 == string::npos) {
                sub_str = content.substr(pos1);
                break;
            } else {
                sub_str = content.substr(pos1, pos2-pos1);
            }
            result.push_back(sub_str);
            pos1 = pos2 + 1;
        }
    }
    return true;
}

bool SaveListToFile(const string &full_file_name, vector<string> &list) {
    string total_str;

    for (size_t i=0; i<list.size(); i++) {
        total_str += list[i] + "\n";
    }

    return SaveFile(full_file_name, total_str);
}

void Trim(string &str, const string &seperators) {
    size_t pos1=0, pos2=0;

    for (pos1=0; pos1<str.size(); pos1++) {
        if (seperators.find(str[pos1]) == string::npos)
            break;
    }
    if (pos1 == str.size()) {
        str = "";
        return;
    }
    for (pos2=str.size()-1; pos2>pos1; pos2--) {
        if (seperators.find(str[pos2]) == string::npos)
            break;
    }
    if (pos2 >= pos1) {
        str = str.substr(pos1, pos2-pos1+1);
    }
}

bool LoadMapFromFile(const string &full_file_name, map<string, string> &result, 
                     const string &seperator) {
    vector<string>      content_list;
    string              line;
    size_t              pos;

    result.clear();
    if (access(full_file_name.c_str(), R_OK) < 0)
        return false;
    LoadListFromFile(full_file_name, content_list);
    for (size_t i=0; i<content_list.size(); i++) {
        pos = content_list[i].find("#");
        if (pos == string::npos) {
            line = content_list[i];
        } else {
            line = content_list[i].substr(0, pos);
        }
        pos = line.find(seperator);
        if (pos == string::npos)
            continue;
        string key = line.substr(0, pos);
        string value = line.substr(pos+1);
        Trim(key);
        Trim(value);
        if (key.size()==0 || value.size()==0)
            continue;
        result.insert(pair<string, string>(key, value));
    }
    return true;
}

size_t ScanDir(const string &path, vector<string> &file_list, bool append_path_flag) {
    string  formated_path = path;

    if (formated_path[formated_path.size()-1] != '/')
        formated_path += "/";
    file_list.clear();
    // TODO use stat
    DIR *pDir = opendir(path.c_str());
    if(pDir == NULL)
        return 0;
    struct dirent *pDirent;
    while((pDirent = readdir(pDir)) != NULL) {
        string file_name = pDirent->d_name;
        if(file_name == "." || file_name == "..")
            continue;
        if (append_path_flag)
            file_name = formated_path + file_name;
        file_list.push_back(file_name);
    }
    closedir(pDir);
    return file_list.size();  
}
} // namespace File
} // namespace kxutil4

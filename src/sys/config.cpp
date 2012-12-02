#include "config.hpp"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <string>

using namespace std;
namespace kxutil4 {

#define SYS_CONFIG_STRIPSTR        " \t\r\n"

Config* Config::instance_      = NULL;
string Config::trim_chars_     = SYS_CONFIG_STRIPSTR;

Config* Config::Instance() {
    if (instance_ == NULL)
    {
        instance_ = new Config();
    }
    return instance_;
}

int Config::LoadFile(const string& conf_file, const string &trim_chars) {
    conf_file_ = conf_file;
    ifstream instr;

    if (trim_chars.size() > 0)
        Config::trim_chars_ = trim_chars;

    section_map_.clear(); // clean old data
    instr.open(conf_file_.c_str(), ios::in);
    if(instr.fail())
        return -1;

    string              section;
    string              key, value;
    map<string, string> config;
    string              line;
    size_t              pos_1, pos_2;

    while( getline(instr, line) ){
        line = TrimChar(line);
        if (line.size() == 0)
            continue;

        pos_1 = line.find('[');
        if ( pos_1 != string::npos ) {
            pos_2 = line.find(']', pos_1);
            if (pos_2 != string::npos) {
                if ( section.size()>0 ) {
                    section_map_[section] = config;
                    config.clear();
                }
                section = line.substr(pos_1 + 1, pos_2 - pos_1 -1);
                continue;
            }
            return -2; 
        }
        // skip comment
        pos_1 = line.find("#");
        if (pos_1 != string::npos)
            line = line.substr(0, pos_1);
        // parse key and value
        pos_1 = line.find("=");
        if (pos_1 == string::npos)
            continue;

        key     = line.substr(0, pos_1);
        value   = line.substr(pos_1 + 1);
        key     = TrimChar(key);
        value   = TrimChar(value);

        if ( key.size()==0 || value.size()==0 )
            continue;

        config[key] = value;
    }
    if ( section.size()>0 )
        section_map_[section] = config;

    instr.close();
    return 0;
}

// return value:
//         < 0, means it can not find in config file
int Config::GetInt(const string &section, const string &name, int* value, int def) {
    string value_string;
    int ret;

    ret = GetStr(section, name, value_string );
    if (ret < 0 || value_string.size() == 0) {
        *value = def;
        return -1;
    }

    *value = atoi(value_string.c_str());
    return 0;
}

int Config::GetStr(const string &section, const string &name, string &value, const string &def) {
    map<string, map<string,string> >::const_iterator iter;

    iter = section_map_.find(section);
    if (iter != section_map_.end()) {
        map<string,string>::const_iterator it = iter->second.find(name);
        if (it != iter->second.end()) {
            value = it->second;
            return 0;
        }
        else {
            value = def;
        }
    }
    return -1;
}

int Config::WriteStr(const string &section, const string &name, const string &value) {
    map<string, string> conf;

    if (section_map_.count(section) == 0) {
        conf[name] = value;
        section_map_[section] = conf;
        return WriteFile();
    }

    section_map_[section][name] = value;
    return WriteFile();
}

int Config::WriteInt(const string &section, const string &name, int value) {
    char   value_buf[12];

    snprintf(value_buf, sizeof(value_buf), "%d", value);
    return WriteStr(section, name, value_buf);
}

int Config::WriteFile() {
    fstream outstr;
    outstr.open(conf_file_.c_str(),fstream::out);
    if(outstr.fail()) 
        return -1;

    string              section;
    map<string, string> config;
    string              output_string;

    for(map<string, map<string, string> >::iterator it=section_map_.begin(); 
            it != section_map_.end(); ++it) {
        section = it->first;
        output_string = string("[") + section + string("]\n");
        outstr.write(output_string.c_str(), output_string.size());
        if(outstr.fail()) 
            return -1;

        config = it->second;
        for(map<string, string>::iterator kv_it=config.begin(); kv_it!=config.end(); ++kv_it) {
            string key   = kv_it->first;
            string value = kv_it->second;
            string output_string = key + "=" + value +string("\n");
            outstr.write(output_string.c_str(), output_string.size());
            if(outstr.fail()) 
                return -2;
        }
        section="";
        config.clear();
    }
    outstr.close();
    return 0;
}

string Config::TrimChar(const string &buf) {
    register const char *tail = buf.c_str();
    register const char *head = buf.c_str();
    const char *begin   = buf.c_str();

    // mark tail
    while (*tail != '\0')
        tail++;
    tail--;

    while (tail >= head && Config::trim_chars_.find(*tail) != string::npos) {
        tail--;
    }

    // mark head
    while(head <= tail && Config::trim_chars_.find(*head) != string::npos) {
        head++;
    }

    if (tail == head && Config::trim_chars_.find(*head) != string::npos)
        return "";

    return buf.substr(head - begin, tail - head + 1);
}

void Config::DumpInfo() {
    string              section;
    map<string, string> config;

    cout<<"Config::DumpInfo " << conf_file_ << " begin. ---------------------" << endl;
    for(map<string, map<string,string> >::iterator it=section_map_.begin(); 
            it != section_map_.end(); ++it ) {
        section = it->first;
        cout << "[" << section << "]" << endl;

        config = it->second;
        for(map<string, string>::iterator kv_it=config.begin(); kv_it!=config.end(); ++kv_it) {
            cout << kv_it->first << "=" << kv_it->second << endl;
        }
        cout<<endl;
        section="";
        config.clear();
    }
    cout << "Config::DumpInfo end. ----------------------" << endl;
}

int Config::GetSections(vector<string> &sections) {
    sections.clear();

    for (map<string, map<string,string> >::iterator it = section_map_.begin();
            it != section_map_.end(); it++) {
        sections.push_back(it->first);
    }
    return 0;
}

} // namespace kxutil4 

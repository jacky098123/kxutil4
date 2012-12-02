#ifndef __KXUTIL4_CONFIG_HPP__
#define __KXUTIL4_CONFIG_HPP__

#include <string>
#include <vector>
#include <map>

using namespace std;
namespace kxutil4 {
class Config {
  public:
    int LoadFile(const string& conf_file, const string &trim_chars="");

    int GetInt(const string &section, const string &name, int* value, int def=-1);
    int GetStr(const string &section, const string &name, string &value, const string &def="");
    int GetSections(vector<string> &sections);
    void DumpInfo();

    int WriteStr(const string &section, const string &name, const string &value);
    int WriteInt(const string &section, const string &name, int value);
    int WriteFile();

    static Config* Instance();

  protected:
    Config() : conf_file_("") { section_map_.clear(); }
    Config(const Config&) {}
    void operator=(const Config&) {}
    string TrimChar(const string &buf);

  protected:
    map<string, map<string,string> > section_map_;
    string                           conf_file_;
    static Config*                   instance_;
    static string                    trim_chars_;
};

} // namespace kxutil4
#endif // __KXUTIL4_CONFIG_HPP__

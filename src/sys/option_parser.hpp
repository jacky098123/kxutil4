/* 
 * date:   2011-02-01
 * author: jacky098123@gmail.com
 */
#ifndef __OPTION_PARSER_HPP__
#define __OPTION_PARSER_HPP__
#include <stdio.h>
#include <string>
#include <map>
#include <set>
#include <vector>

using namespace std;

namespace kxutil4 {
typedef struct Option {
    string short_opt_;
    string long_opt_;
    bool   has_arg_;
    string default_opt_;
    string help_opt_;

    void Clean() {
        short_opt_   = "";
        long_opt_    = "";
        has_arg_     = false; // if false -v; if true -u host
        default_opt_ = "";
        help_opt_    = "";
    }
} Option;

class OptionParser {
  public:
    static const string PROGRAM_NAME;
    static const string OPTION_SHORT_OPT;
    static const string OPTION_LONG_OPT;
    static const string OPTION_ACTION;
    static const string OPTION_DEFAULT;
    static const string OPTION_HELP;
    static const string OPTION_H;
    static const string VALUE_TRUE;
    static const string VALUE_FALSE;
  public:
    OptionParser(const string &usage); 
    bool AddOptions(int count, const Option option_list[]);
    string GetOption(const string &option);
    vector<string> &GetOtherArguments();
    int ParseOneArgument(int argc, char** argv, int &index, set<size_t> &affected_index);
    int ParseArguments(int argc, char **argv);
    bool OptionIsValid(const Option &option);
    void DumpOptions();
    void DumpParsed();
    void Help();
    bool AddOption(const string &short_opt, const string &long_opt, bool has_arg, 
                   const string &default_opt, const string &help);

  private:
    OptionParser() {}
    string Upper(const string &source);
    void AddParsedOption(const Option &option, const string &value);
    string TrimChar(const string &buf);
    string trim_chars_;
    string                  usage_;
    vector<Option>          initialize_options_;
    map<string,string>      parsed_options_;
    vector<string>          other_arguments_;
};
} // namespace kxutil4
#endif // __OPTION_PARSER_HPP__

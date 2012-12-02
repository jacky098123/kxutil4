#include "option_parser.hpp"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <set>

using namespace std;

namespace kxutil4 {
const string OptionParser::PROGRAM_NAME = "\%prog";
const string OptionParser::OPTION_SHORT_OPT = "short_opt";
const string OptionParser::OPTION_LONG_OPT = "long_opt";
const string OptionParser::OPTION_DEFAULT= "default";
const string OptionParser::OPTION_HELP = "help";
const string OptionParser::OPTION_H = "h";
const string OptionParser::VALUE_TRUE= "true";
const string OptionParser::VALUE_FALSE= "false";


OptionParser::OptionParser(const string &usage) {
    trim_chars_ = "\r\n\t ";
    usage_ = usage;
    AddOption("-h", "--help", false, "", "display this message");
}

bool OptionParser::AddOption(const string &short_opt, const string &long_opt, bool has_arg, 
                             const string &default_opt, const string &help_opt) {
    Option     parsed_option;
    if (short_opt[0] == '-') {
        parsed_option.short_opt_        = short_opt.substr(1);
    } else {
        parsed_option.short_opt_        = short_opt;
    }
    if (long_opt.substr(0,2) == "--") {
        parsed_option.long_opt_         = long_opt.substr(2);
    } else {
        parsed_option.long_opt_         = long_opt;
    }
    parsed_option.has_arg_          = has_arg;
    parsed_option.default_opt_      = default_opt;
    parsed_option.help_opt_         = help_opt;

    if (!OptionIsValid(parsed_option)) {
        return false;
    }

    initialize_options_.push_back(parsed_option);
    return true;
}

bool OptionParser::AddOptions(int count, const Option option_list[]) {
    for (int i=0; i<count; i++) {
        Option option = option_list[i];
        if (option.short_opt_[0] == '-') {
            option.short_opt_   = option.short_opt_.substr(1);
        }
        if (option.long_opt_.substr(0,2) == "--") {
            option.long_opt_    = option.long_opt_.substr(2);
        }
        if (!OptionIsValid(option))
            return false;
        initialize_options_.push_back(option);
    }
    return true;
}

bool OptionParser::OptionIsValid(const Option &option) {
    if (option.short_opt_.size() == 0 && option.long_opt_.size() == 0) {
        cout << "invalid option" << endl;
        return false;
    }

    for (size_t i=0; i<initialize_options_.size(); i++) {
        if (option.short_opt_.size() >0 && option.short_opt_ == initialize_options_[i].short_opt_) {
            cout << "short option conflit: " << option.short_opt_ << endl;
            return false;
        }
        if (option.long_opt_.size() >0 && option.long_opt_ == initialize_options_[i].long_opt_) {
            cout << "long option conflit: " << option.long_opt_ << endl;
            return false;
        }
    }
    return true;
}

string OptionParser::GetOption(const string &option) {
    map<string,string>::iterator it = parsed_options_.find(option);
    if (it != parsed_options_.end())
        return it->second;
    return "";
}

vector<string> &OptionParser::GetOtherArguments() {
    return other_arguments_;
}

/*
 * return:
 *      0: display help message
 *     >0: parse succeed
 *     <0: parse failed
 */
int OptionParser::ParseOneArgument(int argc, char** argv, int &index, set<size_t> &affected_index) {
    assert(index < argc);
    string argument = argv[index];
    size_t  arg_length  = argument.size();
    if (argument.size() > 2 && argument.substr(0,2) == "--") {
        argument = argument.substr(2);
        if (argument == OPTION_HELP) {
            Help();
            exit(0);
        }
        for (size_t i=0; i<initialize_options_.size(); i++) {
            size_t long_length = initialize_options_[i].long_opt_.size();
            if (argument == initialize_options_[i].long_opt_) {
                if (initialize_options_[i].has_arg_) {
                    // example: --host 127.0.0.1
                    if (++index >= argc) {
                        return -1;
                    }
                    argument = argv[index];
                    affected_index.insert(i);
                    AddParsedOption(initialize_options_[i], argument);
                    return 1;
                } else {
                    // example: --verbose
                    affected_index.insert(i);
                    AddParsedOption(initialize_options_[i], VALUE_TRUE);
                    return 1;
                }
            } else if (long_length < arg_length 
                       && argument.substr(0,long_length) == initialize_options_[i].long_opt_
                       && argument[long_length] == '=') {
                // example: --host=127.0.0.1
                argument = argument.substr(long_length+1);
                if (argument.size() == 0 || !initialize_options_[i].has_arg_) {
                    return -2;
                }
                affected_index.insert(i);
                AddParsedOption(initialize_options_[i], argument);
                return 1;
            }
        } // for
    } else if (argument.size() > 1 && argument[0] == '-') {
        argument = argument.substr(1);
        if (argument == OPTION_H) {
            Help();
            exit(0);
        }
        size_t arg_length = argument.size();
        for (size_t i=0; i<initialize_options_.size(); i++) {
            size_t short_length = initialize_options_[i].short_opt_.size();
            if (argument == initialize_options_[i].short_opt_) {
                if (initialize_options_[i].has_arg_) {
                    // example: -h 127.0.0.1
                    if (++index >= argc) {
                        return -3;
                    }
                    argument = argv[index];
                    affected_index.insert(i);
                    AddParsedOption(initialize_options_[i], argument);
                    return 2;
                } else {
                    // example: -v
                    affected_index.insert(i);
                    AddParsedOption(initialize_options_[i], VALUE_TRUE);
                    return 2;
                }
            } else if (arg_length > short_length
                       && argument.substr(0, short_length) == initialize_options_[i].short_opt_
                       && argument[short_length] == '=') {
                // example: -h=127.0.0.1
                argument = argument.substr(short_length+1);
                if (argument.size() == 0 || !initialize_options_[i].has_arg_) {
                    return -4;
                }
                affected_index.insert(i);
                AddParsedOption(initialize_options_[i], argument);
                return 2;
            }

        } // for
    }
    return -5;
}

/*
 * return:
 *      0: display help message
 *     >0: parse succeed
 *     <0: parse failed
 */
int OptionParser::ParseArguments(int argc, char** argv) {
    set<size_t>     affected_index; 
    int             ret;
    
    string program_name = basename(argv[0]);
    if (usage_.substr(0,5) == PROGRAM_NAME) { // replace the %prog
        usage_ = program_name + usage_.substr(5);
    }

    for (int i=1; i<argc; i++) { // the first is program_name, skip it
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            Help();
            exit(0);
        }
        if (argv[i][0] == '-') {
            if ((ret = ParseOneArgument(argc, argv, i, affected_index)) < 0) {
                return ret;
            }
        } else {
            other_arguments_.push_back(argv[i]);
        }
    }

    // add default option if not set for command line
    for (size_t i=0; i<initialize_options_.size(); i++) {
        if (affected_index.count(i) != 0)
            continue;
        if (initialize_options_[i].has_arg_ && initialize_options_[i].default_opt_.size() > 0)
            AddParsedOption(initialize_options_[i], initialize_options_[i].default_opt_);
    }
    return 1;
}

void OptionParser::Help() {
    char    buffer[1024];
    string  value;

    cout << "Usage: " << usage_ << endl << endl;
    for (size_t i=0; i<initialize_options_.size(); i++) {
        if (initialize_options_[i].has_arg_) {
            if (initialize_options_[i].long_opt_.size() > 0)
                value = Upper(initialize_options_[i].long_opt_);
            else
                value = Upper(initialize_options_[i].short_opt_);
        }
        // add short_opt_
        memset(buffer, 0x00, sizeof(buffer));
        if (initialize_options_[i].short_opt_.size() > 0 && initialize_options_[i].has_arg_) {
            snprintf(buffer, sizeof(buffer), "  -%s %s",
                     initialize_options_[i].short_opt_.c_str(), value.c_str());
        } else if (initialize_options_[i].short_opt_.size() > 0) {
            snprintf(buffer, sizeof(buffer), "  -%s", initialize_options_[i].short_opt_.c_str());
        }
        size_t ShortSpace=7;
        if (strlen(buffer) >= ShortSpace) { // has short_opt_
            strcat(buffer, ",");
        } else if (strlen(buffer) > 0) { // has short_opt_
            size_t begin = strlen(buffer);
            size_t left  = ShortSpace - begin;
            buffer[begin] = ',';
            for (size_t i=1; i<=left; i++) {
                buffer[begin + i] = ' ';
            }
        } else {
            sprintf(buffer, "%*s", int(ShortSpace + 1), " ");
        }

        // add long_opt_
        if (initialize_options_[i].long_opt_.size() > 0) {
            int offset = strlen(buffer);
            if (initialize_options_[i].has_arg_) {
                snprintf(buffer+offset, sizeof(buffer)-offset, "--%s=%s",
                         initialize_options_[i].long_opt_.c_str(), value.c_str());
            } else {
                snprintf(buffer+offset, sizeof(buffer)-offset, "--%s",
                         initialize_options_[i].long_opt_.c_str());
            }
        }
        printf("%s", buffer);
        if (strlen(buffer) > 24) {
            printf("\n%24s%s\n", " ", initialize_options_[i].help_opt_.c_str());
        } else {
            int left = 24 - strlen(buffer);
            printf("%*s%s\n", left, " ", initialize_options_[i].help_opt_.c_str());
        }
    }
}

string OptionParser::Upper(const string &source) {
    string destination = source;
    int    offset = 'a' - 'A';
    for (size_t i=0; i<destination.size(); i++) {
        if (destination[i] >= 'a' && destination[i] <= 'z') {
            destination[i] = destination[i] - offset;
        }
    }
    return destination;
}

void OptionParser::AddParsedOption(const Option &option, const string &value) {
    if (option.short_opt_.size() > 0)
        parsed_options_.insert(map<string,string>::value_type(option.short_opt_, value));
    if (option.long_opt_.size() > 0)
        parsed_options_.insert(map<string,string>::value_type(option.long_opt_, value));
}

void OptionParser::DumpOptions() {
    cout << "DumpOptions begin -----------------------------------" << endl;
    // skip the first help option
    for (size_t i=1; i<initialize_options_.size(); i++) {
        cout << "index: " << i << endl;
        cout << "\tshort_opt_:" << initialize_options_[i].short_opt_ << endl;
        cout << "\tlong_opt_:" << initialize_options_[i].long_opt_ << endl;
        cout << "\tdefault_opt_:" << initialize_options_[i].default_opt_ << endl;
        cout << "\thelp_opt_:" << initialize_options_[i].help_opt_ << endl;
        cout << "\thas_arg_:" << initialize_options_[i].has_arg_ << endl;
    }
    cout << "DumpOptions end ---------------" << endl;
}

void OptionParser::DumpParsed() {
    cout << "DumpParsed begin ----------------------------------" << endl;
    int i=0;
    for (map<string,string>::iterator it=parsed_options_.begin();
         it!=parsed_options_.end(); it++) {
        cout << "index: " << i++ << endl;
        cout << "\t" << it->first << "\t" << it->second << endl;
    }
    cout << "DumpParsed end ---------" << endl;
}

string OptionParser::TrimChar(const string &buf) {
    size_t position_1;
    size_t position_2;
    // right trim
    position_2 = buf.size() - 1;
    while (trim_chars_.find(buf[position_2]) != string::npos) {
        position_2--;
        if (position_2 == 0)
            break;
    }

    // light trim
    position_1 = 0;
    while (trim_chars_.find(buf[position_1]) != string::npos) {
        position_1++;
        if (position_1 == buf.size() - 1)
            break;
    }

    if (position_1 >= position_2)
        return "";
    return buf.substr(position_1, position_2 - position_1 + 1);
}

} // namespace kxutil4

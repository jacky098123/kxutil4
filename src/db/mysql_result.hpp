#ifndef __MYSQL_RESULT_HPP__
#define __MYSQL_RESULT_HPP__

#include <string>
#include <vector>

namespace kxutil4 {

class MySQLResult {
  public:
    MySQLResult() {
        Initialize();
    }

    void Initialize() {
        error_no        = 0;
        error_str       = "";
        affected_rows   = 0;
        rows.clear();
        result_set.clear();
    }

  public:
    int             error_no;
    std::string     error_str;
    int             affected_rows;
    std::vector<std::string>                rows;
    std::vector<std::vector<std::string> >  result_set;
};
} // namespace kxutil4
#endif // __MYSQL_RESULT_HPP__

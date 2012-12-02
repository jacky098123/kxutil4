/*
 * test case: 
 *   timeout and reconnect
 *
 */
#ifndef __KXUTIL3_MYSQL_CONNECTION_HPP__
#define __KXUTIL3_MYSQL_CONNECTION_HPP__

#include <string>
#include <vector>
#include <map>
#include <mysql/mysql.h>

#include "log/util_log.hpp"
#include "mysql_result.hpp"

using namespace std;

namespace kxutil4 {

class MySQLConnection: public UtilLog {
  public:
    MySQLConnection() {}
    virtual ~MySQLConnection() {}

    static MySQLConnection *CreateInstance(const std::string &host, const std::string &user,
                                            const std::string &passwd, const std::string &database, 
                                            int port=3306, const std::string &encoding="utf8");

    int Initialize(const std::string &host, const std::string &user, const std::string &passwd,
                        const std::string &database, int port=3306, const std::string &encoding="utf8");
    int Finish();

    int GetInsertID();
    int Execute(const std::string &sql, int &error_no, std::string &error_str);
    int Execute(const std::string &sql);
    int Query(const std::string &sql, MySQLResult &result);
    int DumpSql(std::string &sql, int max_rows = 10, int format=1);
    void Dump();

  private:
    bool ApplyConnection();
    bool Connect(MYSQL *handle);

  private: 
    std::string host_;
    std::string user_; 
    std::string passwd_;
    std::string default_database_;
    int    port_;
    std::string encoding_;

    MYSQL*     mysql_connection_;
};
} // namespace kxutil4
#endif // __KXUTIL3_MYSQL_CONNECTION_HPP__

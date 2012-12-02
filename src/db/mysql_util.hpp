#ifndef __KXUTIL3_MYSQL_UTIL_HPP__
#define __KXUTIL3_MYSQL_UTIL_HPP__
#include <string>
#include <vector>
#include "mysql_connection.hpp"

using namespace std;
namespace kxutil4 {
namespace mysql_util {

std::string EscapeString(const std::string &input);
int GetTableRowCount(MySQLConnection *connection, const std::string &table_name);

} // namespace mysql_util
} // namespace kxutil4
#endif // __KXUTIL3_MYSQL_UTIL_HPP__

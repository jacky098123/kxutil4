#include "mysql_util.hpp"

#include <assert.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <mysql/mysql.h>

namespace kxutil4 {
namespace mysql_util {

std::string EscapeString(const std::string &input) {    
    if(input.length()==0)   
        return input; 
    char * buf = new char[input.length() * 2];
    unsigned long  nLen = mysql_escape_string(buf, input.c_str(), input.length());
    buf[nLen] = '\0'; 
    std::string result = buf;  
    delete [] buf;  
    return result;
}


int GetTableRowCount(MySQLConnection *connection, const std::string &table_name) {
    assert(connection != NULL || table_name.size() > 0);

    MySQLResult mysql_result;

    std::string sql = "select count(*) from " + table_name + ";";

    int ret = connection->Query(sql, mysql_result);
    if (ret < 0)
        return -1;

    if (mysql_result.result_set.size() == 0 || mysql_result.result_set[0].size() == 0) {
        return -2;
    }

    return atoi(mysql_result.result_set[0][0].c_str());
}

} // namespace mysql_util
} // namespace kxutil4

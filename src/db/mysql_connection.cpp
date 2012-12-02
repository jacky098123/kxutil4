#include "mysql_connection.hpp"
#include <mysql/mysql.h>
#include "log/util_log.hpp"

using namespace std;

namespace kxutil4 {

#define MYSQL_SAFESTR(p) (p == NULL) ? "":p

int MySQLConnection::Finish() {
    if (mysql_connection_ != NULL) {
        mysql_close(mysql_connection_);
        delete mysql_connection_;
        mysql_connection_ = NULL;
    }
    return 0;
}

MySQLConnection *MySQLConnection::CreateInstance(const std::string &host, const std::string &user,
                                        const std::string &passwd, const std::string &database, 
                                        int port, const std::string &encoding) {
    MySQLConnection *object = new MySQLConnection();
    if (object == NULL) {
        cout << "alloc MySQLConnection error." << endl;
        return NULL;
    }
    object->Initialize(host, user, passwd, database, port, encoding);
    return object;
}

int MySQLConnection::Initialize(const std::string &host, const std::string &user, const std::string &passwd,
                    const std::string &database, int port, const std::string &encoding) {

    if (host.size()==0 || user.size()==0 || passwd.size()==0 || database.size()==0 
        || encoding.size() ==0 || port ==0)
        return -1;

    host_   = host;
    user_   = user;
    passwd_ = passwd;
    default_database_ = database;
    port_     = port;
    encoding_ = encoding;

    mysql_connection_ = new MYSQL;
    if (mysql_connection_ == NULL) {
        cout << "can not alloc MYSQL" << endl;
        return -2;
    }

    if (!Connect(mysql_connection_)) {
        return -3;
    }
    return 0;
}


int MySQLConnection::GetInsertID() {
    return mysql_insert_id(mysql_connection_);
}

/**
 * used for insert/update/delete
 * return: affected rows
 *     >= 0: succeed; <0: fail
 */
int MySQLConnection::Execute(const std::string &sql, int &error_no, std::string& error_str) {
    int     affected_rows   = 0;
    int     try_times       = 0;

    error_str   = "";
    if (!ApplyConnection())
        return -2;

    UTIL_LOG_DEBUG(sql.c_str());

    while (try_times++ < 3) {
        if (mysql_query(mysql_connection_, sql.c_str()) == 0) {
            affected_rows = mysql_affected_rows(mysql_connection_);
            break;
        } else {
            error_no = mysql_errno(mysql_connection_);
            affected_rows = -error_no;
            error_str = mysql_error(mysql_connection_);
            UTIL_LOG_WARN_FMT("MySQLConnection: errno:%d, str: %s", error_no, error_str.c_str());
            if (error_no == 2006) { // timeout, retry
                Connect(mysql_connection_);
            } else { // handle error, close it.
                cout << "ERROR: MySQLConnection error, close connection: " << error_str << endl;
                mysql_close(mysql_connection_);
                delete mysql_connection_;
                mysql_connection_ = NULL;
                break;
            }
        }
    }
    return affected_rows;
}

/** 
 * used for insert/update/delete 
 * return: affected rows
 *     >= 0: succeed; <0: fail
 */
int MySQLConnection::Execute(const std::string &sql) {
    int error_no;
    std::string error_str;
    return Execute(sql, error_no, error_str);
}

/*
 * used for select
 * return: <0, failed; 
 */
int MySQLConnection::Query(const std::string &sql, MySQLResult &result) {
    int try_times   = 0;
    result.Initialize();

    if (!ApplyConnection())
        return -2;

    UTIL_LOG_DEBUG(sql.c_str());

    while (try_times++ < 3) {
        if (mysql_query(mysql_connection_, sql.c_str()) == 0) {
            result.error_no = 0;
            MYSQL_RES *res;
            MYSQL_ROW row;
            if((res=mysql_store_result(mysql_connection_)) != NULL) { // get all row
                int num_field = mysql_num_fields(res);
                int num_row   = mysql_num_rows(res);
                result.result_set.resize(num_row);
                for (int i=0; i<num_row; i++) { // get one row
                    vector<string> &one_row = result.result_set[i];
                    one_row.resize(num_field);
                    if((row = mysql_fetch_row(res)) != NULL){
                        for(int j=0; j<num_field; j++){
                            one_row[j] = MYSQL_SAFESTR(row[j]);
                        }
                    }
                }
                MYSQL_FIELD *fields; // get all column name
                fields = mysql_fetch_fields(res);
                for (int i=0; i<num_field; i++) {
                    result.rows.push_back(fields[i].name);
                }
                mysql_free_result(res);
                break;
            }
        } else {
            result.error_no = mysql_errno(mysql_connection_);
            result.error_str = mysql_error(mysql_connection_);
            UTIL_LOG_WARN_FMT("MySQLConnection: errno: %d, str: %s", result.error_no, result.error_str.c_str());
            if (result.error_no == 2006) { // timeout, retry
                Connect(mysql_connection_);
            } else { // handle error, close it.
                cout << "ERROR: MySQLConnection error, close connection: " << result.error_str << endl;
                mysql_close(mysql_connection_);
                delete mysql_connection_;
                mysql_connection_ = NULL;
                break;
            }
        }
    } // while 

    if (result.error_no == 0)
        return 0;
    return -result.error_no;
}

bool MySQLConnection::ApplyConnection() {
    if (mysql_connection_ != NULL)
        return true;

    mysql_connection_ = new MYSQL;
    if (mysql_connection_ == NULL) {
        cout << "can not alloc MYSQL" << endl;
        return false;
    }
    return Connect(mysql_connection_);
}

bool MySQLConnection::Connect(MYSQL * handle) {
    mysql_init(handle);
    if (mysql_real_connect(handle, host_.c_str(), user_.c_str(), passwd_.c_str(),
                           default_database_.c_str(), port_, NULL, 0) == 0) {
        cout << "can not connect to database: " << host_ << ", " << user_ << ", " << passwd_ 
            << ", " << default_database_ << ", "  << port_ << endl;
        return false;
    }
    mysql_set_character_set(handle, encoding_.c_str());
    return true;
}

/*
 * format: 0: vector; 1: map
 */
int MySQLConnection::DumpSql(std::string &sql, int max_rows, int format) {
    MySQLResult mysql_result;
    int ret = Query(sql, mysql_result);
    if (ret < 0) {
        printf("return: %d, error_str: %s\n", ret, mysql_result.error_str.c_str());
        return ret;
    }

    printf("Dump result set: [%s]\n", sql.c_str());
    if (format == 0) {
        for (size_t i=0; i<mysql_result.result_set.size() && i<(size_t)max_rows; i++) {
            printf("\ti: %zu\n", i);
            for (size_t j=0; j<mysql_result.result_set[i].size(); j++) {
                printf("\t\t%s", mysql_result.result_set[i][j].c_str());
            }
            printf("\n");
        }
    } else {
        for (size_t i=0; i<mysql_result.result_set.size() && i<(size_t)max_rows; i++) {
            printf("\ti: %zu\n", i);
            for (size_t j=0; j<mysql_result.rows.size(); j++) {
                printf("\t\t%s ==> %s\n", mysql_result.rows[j].c_str(),
                       mysql_result.result_set[i][j].c_str());
            }
        }
    }
    return ret;
}

void MySQLConnection::Dump() {
    cout <<"MySQLConnection info: " << host_ << ", " << user_ << ", " << passwd_ << ", " 
        << default_database_ << ", " <<  port_ << ", " <<  encoding_ << endl;
}

} // namespace kxutil4

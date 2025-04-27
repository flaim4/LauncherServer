#ifndef DATABSE_HELPER
#define DATABSE_HELPER

#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

class DatabaseHelper
{
private:
    sql::mysql::MySQL_Driver *driver;
    std::unique_ptr<sql::Connection> con;
    std::string host;
    std::string user;
    std::string pass;

    void ensureConnected();

    void DatabaseHelper::ensureConnected() {
        if (!con || con->isClosed()) {
            con.reset(driver->connect(host, user, pass));
        }
    }

public:
    DatabaseHelper(const std::string& host, const std::string& user, const std::string& pass);
    ~DatabaseHelper();
    std::unique_ptr<sql::Statement> createStatement();
    std::unique_ptr<sql::PreparedStatement> prepareStatement(const std::string& query);
    void DatabaseHelper::setSchema(const std::string& dbName);
};

#endif
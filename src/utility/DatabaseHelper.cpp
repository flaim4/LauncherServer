#include "DatabaseHelper.hpp"

DatabaseHelper::DatabaseHelper(const std::string& host, const std::string& user, const std::string& pass) 
    : host(host), user(user), pass(pass)
{
    driver = sql::mysql::get_mysql_driver_instance();
    con = nullptr;
}

DatabaseHelper::~DatabaseHelper()
{

}


std::unique_ptr<sql::Statement> DatabaseHelper::createStatement() {
    ensureConnected();
    return std::unique_ptr<sql::Statement>(con->createStatement());
}

std::unique_ptr<sql::PreparedStatement> DatabaseHelper::prepareStatement(const std::string& query) {
    ensureConnected();
    return std::unique_ptr<sql::PreparedStatement>(con->prepareStatement(query));
}

void DatabaseHelper::setSchema(const std::string& dbName) {
    ensureConnected();
    con->setSchema(dbName);
}
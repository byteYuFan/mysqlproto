//
// Created by wangyufan on 2024-01-21.
//

#ifndef PROTOOFMYSQL_HZMYSQLCLIENT_H
#define PROTOOFMYSQL_HZMYSQLCLIENT_H


#include <string>
#include <utility>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "hzporto/hzmysql.h"
#include "hzporto/HandshakePacket.h"
#include "hzporto/AuthPacket.h"
#include "utils/SecurityUtil.h"
#include "hzporto/OKPackage.h"
#include "hzporto/ErrorPacket.h"
#include "hzporto/hz_protocol.h"
#include "hzporto/QuitPacket.h"
#include "hzporto/QueryPacket.h"
#include "hzporto/ColumnCountPacket.h"
#include "hzporto/ColumnDefinitionPacket.h"
#include "hzporto/ResultSetRowPacket.h"
#include "hzporto/EOFPacket.h"

class HzDealCommand;

class QueryResult {
    friend HzMySQLClient;

    friend HzDealCommand;

private:
    std::shared_ptr<std::vector<std::shared_ptr<std::vector<char>>>> column_definition_;
    std::shared_ptr<std::vector<std::shared_ptr<std::vector<char>>>> column_data_;

    QueryResult() {
        column_definition_ = std::make_shared<std::vector<std::shared_ptr<std::vector<char>>>>();
        column_data_ = std::make_shared<std::vector<std::shared_ptr<std::vector<char>>>>();
    }

    QueryResult &operator=(const QueryResult &other) {
        if (this != &other) {
            column_definition_ = std::make_shared<std::vector<std::shared_ptr<std::vector<char>>>>(
                    *other.column_definition_);

            column_data_ = std::make_shared<std::vector<std::shared_ptr<std::vector<char>>>>(*other.column_data_);
        }
        return *this;
    }
};

class DatabaseConfig {
public:
    std::string host;
    std::string user;
    std::string password;
    std::string database;
    int port = 3306;

    DatabaseConfig(std::string host, std::string user, std::string password,
                   std::string database, int port = 3306)
            : host(std::move(host)), user(std::move(user)), password(std::move(password)),
              database(std::move(database)), port(port) {}

    DatabaseConfig() = default;

    void displayConfig() const {
        std::cout << "Host: " << host << "\n";
        std::cout << "User: " << user << "\n";
        std::cout << "Password: " << password << "\n";
        std::cout << "Database: " << database << "\n";
        std::cout << "Port: " << port << "\n";
    }

};

class HzMySQLClient {
    friend class HzDealCommand;

private:
    int socket_fd_{2};
    bool status_ = false;
    const std::string host_;
    const std::string user_;
    const std::string password_;
    const std::string database_;
    const int port_;
    int errno_code_{0};
    std::string error_message_;
    QueryResult query_result_;
public:
    long thread_id_{};
    std::string version_;
public:
    HzMySQLClient(std::string host, std::string user, std::string password, std::string database, int port = 3306)
            : host_(std::move(host)), user_(std::move(user)), password_(std::move(password)),
              database_(std::move(database)), port_(port) {
    }

    explicit HzMySQLClient(DatabaseConfig &databaseConfig) : host_(std::move(databaseConfig.host)),
                                                             user_(std::move(databaseConfig.user)),
                                                             password_(std::move(databaseConfig.password)),
                                                             database_(std::move(databaseConfig.database)),
                                                             port_(databaseConfig.port) {

    }

    HzMySQLClient(HzMySQLClient &) = delete;

    void operator=(HzMySQLClient &) = delete;

    ~HzMySQLClient() = default;

public:
    void getErrorInfo() const;

    bool conn();

    bool quit();

    bool query(const std::string &query);

private:
    std::vector<std::vector<char>> dealHandShakePacket(std::vector<char> &mysql_data);

    std::vector<char> produceAuthPacket(const std::vector<char> &rand1, const std::vector<char> &rand2);

    static std::vector<char> produceQuitPackage();

    static std::vector<char> buildQueryPacket(const std::string &query_str);

    bool sendQueryPacket(const std::string &query_str);

    bool parseResult(std::vector<char> &);

    static ColumnCountPacket processColumnCountPacket(std::vector<char> &);

    std::vector<ColumnDefinitionPacket> parseColumnDefinition(int column_count, std::vector<char> &byte);

    static EOFPacket parseEOFPacket(std::vector<char> &byte);

    static std::vector<std::shared_ptr<ResultSetRowPacket>> getResultSetRows(int, std::vector<char> &byte);
};


#endif //PROTOOFMYSQL_HZMYSQLCLIENT_H

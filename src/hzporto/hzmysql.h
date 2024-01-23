//
// Created by wangyufan on 2024-01-21.
//

#ifndef PROTOOFMYSQL_HZMYSQL_H
#define PROTOOFMYSQL_HZMYSQL_H

class DatabaseConfig;

class HzMySQLClient;

bool resolveHostname(const std::string &hostname, std::string &ipAddress);

bool readConfigFromFile(const std::string &configFile, std::string &host, std::string &user,
                        std::string &password, std::string &database, int &port);

bool readConfigFromFile(const std::string &configFile, DatabaseConfig &databaseConfig);

bool parseCommandLineArgs(int argc, char **argv, std::string &host, std::string &user,
                          std::string &password, std::string &database, int &port);

bool parseCommandLineArgs(int argc, char **argv, DatabaseConfig &databaseConfig);

void showMySQLConnInfo(const std::string &version, long thread_id);

bool testIfEXIT(const std::string &userInput);


void processUserInput(HzMySQLClient &hzMySqlClient);

#endif //PROTOOFMYSQL_HZMYSQL_H

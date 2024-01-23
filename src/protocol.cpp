//
// Created by wangyufan on 2024-01-15.
//
#include <iostream>
#include <string>
#include "HzMySQLClient.h"

int main(int argc, char **argv) {
    DatabaseConfig databaseConfig;
    std::string configFile = PROJECT_ROOT_PATH + "src/config/config.yml";
    if (!readConfigFromFile(configFile, databaseConfig))return 1;
    if (!parseCommandLineArgs(argc, argv, databaseConfig))return 1;
    if (databaseConfig.host.empty() || databaseConfig.user.empty() || databaseConfig.password.empty() ||
        databaseConfig.database.empty()) {
        std::cerr << "Missing required parameters.\n";
        std::cerr << "Usage: " << argv[0] << " -h <host> -u <user> -p <password> -d <database> -t <port>\n";
        return 1;
    }
    HzMySQLClient hzMySqlClient(databaseConfig);
    if (!hzMySqlClient.conn()) {
        hzMySqlClient.getErrorInfo();
        hzMySqlClient.quit();
        return 0;
    }
    showMySQLConnInfo(hzMySqlClient.version_, hzMySqlClient.thread_id_);
    processUserInput(hzMySqlClient);
    return 0;
}



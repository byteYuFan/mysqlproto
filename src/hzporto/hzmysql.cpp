//
// Created by wangyufan on 2024-01-21.
#include <unistd.h>
#include "../HzMySQLClient.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <netdb.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "hzmysql.h"
#include "deal_command.h"

bool resolveHostname(const std::string &hostname, std::string &ipAddress) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname.c_str(), NULL, &hints, &res) != 0) {
        return false; // 解析失败
    }

    void *addr;
    char ipstr[INET6_ADDRSTRLEN];

    // 获取IP地址
    if (res->ai_family == AF_INET) { // IPv4
        struct sockaddr_in *ipv4 = (struct sockaddr_in *) res->ai_addr;
        addr = &(ipv4->sin_addr);
    } else { // IPv6
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) res->ai_addr;
        addr = &(ipv6->sin6_addr);
    }

    // 将地址转换为字符串形式
    inet_ntop(res->ai_family, addr, ipstr, sizeof ipstr);
    ipAddress = ipstr;

    freeaddrinfo(res);

    return true; // 解析成功
}

bool readConfigFromFile(const std::string &configFile, std::string &host, std::string &user,
                        std::string &password, std::string &database, int &port) {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << configFile << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            value.erase(std::remove_if(value.begin(), value.end(),
                                       [](char c) { return c == '\n' || c == '\r'; }),
                        value.end());
            //todo 可能会出现异常

            if (key == "host") {
                host = value;
            } else if (key == "user") {
                user = value;
            } else if (key == "password") {
                password = value;
            } else if (key == "database") {
                database = value;
            } else if (key == "port") {
                try {
                    port = std::stoi(value);
                } catch (const std::invalid_argument &e) {
                    std::cerr << "Invalid port value in config file: " << e.what() << std::endl;
                    return false;
                } catch (const std::out_of_range &e) {
                    std::cerr << "Out of range port value in config file: " << e.what() << std::endl;
                    return false;
                }
            }
        }
    }

    file.close();
    return true;
}

bool readConfigFromFile(const std::string &configFile, DatabaseConfig &databaseConfig) {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << configFile << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            value.erase(std::remove_if(value.begin(), value.end(),
                                       [](char c) { return c == '\n' || c == '\r'; }),
                        value.end());
            //todo 可能会出现异常

            if (key == "host") {
                databaseConfig.host = value;
            } else if (key == "user") {
                databaseConfig.user = value;
            } else if (key == "password") {
                databaseConfig.password = value;
            } else if (key == "database") {
                databaseConfig.database = value;
            } else if (key == "port") {
                try {
                    databaseConfig.port = std::stoi(value);
                } catch (const std::invalid_argument &e) {
                    std::cerr << "Invalid port value in config file: " << e.what() << std::endl;
                    return false;
                } catch (const std::out_of_range &e) {
                    std::cerr << "Out of range port value in config file: " << e.what() << std::endl;
                    return false;
                }
            }
        }
    }

    file.close();
    return true;
}

// 命令行参数解析函数
bool parseCommandLineArgs(int argc, char **argv, std::string &host, std::string &user,
                          std::string &password, std::string &database, int &port) {
    int opt;
    while ((opt = getopt(argc, argv, "h:u:p:d:t:")) != -1) {
        try {
            switch (opt) {
                case 'h':
                    host = optarg;
                    break;
                case 'u':
                    user = optarg;
                    break;
                case 'p':
                    password = optarg;
                    break;
                case 'd':
                    database = optarg;
                    break;
                case 't':
                    port = std::stoi(optarg);
                    break;
                default:
                    throw std::invalid_argument("Invalid option");
            }
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid argument for option: " << static_cast<char>(opt) << std::endl;
            return false;
        } catch (const std::out_of_range &e) {
            std::cerr << "Out of range value for option: " << static_cast<char>(opt) << std::endl;
            return false;
        }
    }

    return true;
}

bool parseCommandLineArgs(int argc, char **argv, DatabaseConfig &databaseConfig) {
    int opt;
    while ((opt = getopt(argc, argv, "h:u:p:d:t:")) != -1) {
        try {
            switch (opt) {
                case 'h':
                    databaseConfig.host = optarg;
                    break;
                case 'u':
                    databaseConfig.user = optarg;
                    break;
                case 'p':
                    databaseConfig.password = optarg;
                    break;
                case 'd':
                    databaseConfig.database = optarg;
                    break;
                case 't':
                    databaseConfig.port = std::stoi(optarg);
                    break;
                default:
                    throw std::invalid_argument("Invalid option");
            }
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid argument for option: " << static_cast<char>(opt) << std::endl;
            return false;
        } catch (const std::out_of_range &e) {
            std::cerr << "Out of range value for option: " << static_cast<char>(opt) << std::endl;
            return false;
        }
    }

    return true;
}

void showMySQLConnInfo(const std::string &version, long thread_id) {
    std::cout << "Connected successfully!\n";

    std::cout << "mysql: [Warning] Using a password on the command line interface can be insecure.\n";
    std::cout << "Welcome to the MySQL monitor.  Commands end with ; or \\g.\n";
    printf("Your MySQL connection id is %ld .\n", thread_id);
    printf("Server version: %s MySQL Community Server - GPL", version.data());
    std::cout << "\n";
    std::cout << "Copyright (c) 2000, 2023, Oracle and/or its affiliates.\n";
    std::cout << "\n";
    std::cout << "Oracle is a registered trademark of Oracle Corporation and/or its\n";
    std::cout << "affiliates. Other names may be trademarks of their respective\n";
    std::cout << "owners.\n";
    std::cout << "\n";
    std::cout << "Type 'help;' or '\\h' for help. Type '\\c' to clear the current input statement.\n";
    std::cout << "\n";
}


void processUserInput(HzMySQLClient &hzMySqlClient) {
    bool multiLineInput = false;
    std::string userInput;

    while (true) {
        if (!multiLineInput) {
            std::cout << "mysql> ";
        } else {
            std::cout << "    -> ";
        }

        std::string line;
        std::getline(std::cin, line);

        if (testIfEXIT(line)) {
            printf("Bye!\n");
            hzMySqlClient.quit();
            break;
        }

        if (line.empty()) {
            userInput += " ";
            multiLineInput = false;
        } else {
            // 拼接用户输入到整个命令字符串
            userInput += line;

            // 检查是否以分号或 \g 结尾
            size_t lastCharIndex = userInput.find_last_not_of(" \t\n\r");
            if (lastCharIndex != std::string::npos &&
                (userInput[lastCharIndex] == ';' || userInput[lastCharIndex] == '\\' ||
                 userInput[lastCharIndex] == 'g')) {
                HzDealCommand hzDealCommand(hzMySqlClient);
                if (!hzDealCommand.dealQuery(userInput))
                    hzMySqlClient.getErrorInfo();
                userInput.clear();
                multiLineInput = false;
            } else {
                // 如果没有以分号或 \g 结尾，表示多行输入
                multiLineInput = true;
            }
        }
    }
}


bool testIfEXIT(const std::string &userInput) {
    std::string EXIT = "EXIT";
    for (int i = 0; i < 4; i++) {
        if (toupper(userInput[i]) != EXIT[i])
            return false;
    }
    return true;

}
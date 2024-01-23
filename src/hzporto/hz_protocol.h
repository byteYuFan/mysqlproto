//
// Created by wangyufan on 2024-01-15.
//

#ifndef PROTOOFMYSQL_HZ_PROTOCOL_H
#define PROTOOFMYSQL_HZ_PROTOCOL_H

#define HZ_DEBUGE 1

struct Capabilities {
public:
    static const int CLIENT_LONG_PASSWORD = 1;
    static const int CLIENT_FOUND_ROWS = 2;
    static const int CLIENT_LONG_FLAG = 4;
    static const int CLIENT_CONNECT_WITH_DB = 8;
    static const int CLIENT_NO_SCHEMA = 16;
    static const int CLIENT_COMPRESS = 32;
    static const int CLIENT_ODBC = 64;
    static const int CLIENT_LOCAL_FILES = 128;
    static const int CLIENT_IGNORE_SPACE = 256;
    static const int CLIENT_PROTOCOL_41 = 512;
    static const int CLIENT_INTERACTIVE = 1024;
    static const int CLIENT_SSL = 2048;
    static const int CLIENT_IGNORE_SIGPIPE = 4096;
    static const int CLIENT_TRANSACTIONS = 8192;
    static const int CLIENT_RESERVED = 16384;
    static const int CLIENT_SECURE_CONNECTION = 32768;
    static const int CLIENT_MULTI_STATEMENTS = 65536;
    static const int CLIENT_MULTI_RESULTS = 131072;

public:
    static int getClientCapabilities() {
        int flag = 0;
        flag |= CLIENT_LONG_PASSWORD;
        flag |= CLIENT_FOUND_ROWS;
        flag |= CLIENT_LONG_FLAG;
        flag |= CLIENT_CONNECT_WITH_DB;
        flag |= CLIENT_ODBC;
        flag |= CLIENT_IGNORE_SPACE;
        flag |= CLIENT_PROTOCOL_41;
        flag |= CLIENT_INTERACTIVE;
        flag |= CLIENT_IGNORE_SIGPIPE;
        flag |= CLIENT_TRANSACTIONS;
        flag |= CLIENT_SECURE_CONNECTION;
        return flag;
    }
};

const std::string PROJECT_ROOT_PATH = "/home/wyf/clion/protocol/";
#endif //PROTOOFMYSQL_HZ_PROTOCOL_H

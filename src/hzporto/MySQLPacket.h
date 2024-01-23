//
// Created by wangyufan on 2024-01-15.
//

#ifndef PROTOOFMYSQL_MYSQLPACKET_H
#define PROTOOFMYSQL_MYSQLPACKET_H

#include <string>
#include <vector>

class MySQLPacket {
public:
    const char COM_SLEEP = 0;
    const char COM_QUIT = 1;
    const char COM_INIT_DB = 2;
    const char COM_QUERY = 3;
    const char COM_FIELD_LIST = 4;
    const char COM_CREATE_DB = 5;
    const char COM_DROP_DB = 6;
    const char COM_REFRESH = 7;
    const char COM_SHUTDOWN = 8;
    const char COM_STATISTICS = 9;
    const char COM_PROCESS_INFO = 10;
    const char COM_CONNECT = 11;
    const char COM_PROCESS_KILL = 12;
    const char COM_DEBUG = 13;
    const char COM_PING = 14;
    const char COM_TIME = 15;
    const char COM_DELAYED_INSERT = 16;
    const char COM_CHANGE_USER = 17;
    const char COM_BINLOG_DUMP = 18;
    const char COM_TABLE_DUMP = 19;
    const char COM_CONNECT_OUT = 20;
    const char COM_REGISTER_SLAVE = 21;
    const char COM_STMT_PREPARE = 22;
    const char COM_STMT_EXECUTE = 23;
    const char COM_STMT_SEND_LONG_DATA = 24;
    const char COM_STMT_CLOSE = 25;
    const char COM_STMT_RESET = 26;
    const char COM_SET_OPTION = 27;
    const char COM_STMT_FETCH = 28;

    int packet_length_{};
    char packet_id_{};

    [[nodiscard]] virtual int calculatePacketSize() const = 0;

protected:
    [[nodiscard]] virtual std::string getPacketInfo() const = 0;

public:
    [[maybe_unused]] virtual void read(const char *data,unsigned length) = 0;
    virtual void read(std::vector<char>&)=0;
    virtual void write(std::vector<char> &buffer) const = 0;
    virtual void show() const{};
};


#endif //PROTOOFMYSQL_MYSQLPACKET_H

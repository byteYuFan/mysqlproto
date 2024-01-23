//
// Created by wangyufan on 2024-01-22.
//

#ifndef PROTOOFMYSQL_QUERYPACKET_H
#define PROTOOFMYSQL_QUERYPACKET_H

#include "MySQLPacket.h"

class QueryPacket : public MySQLPacket {
    friend class HzMySQLClient;

private:
    char flag_;
    std::vector<char> message_;

    [[nodiscard]]  int calculatePacketSize() const override;

protected:
    [[nodiscard]]  std::string getPacketInfo() const override;

public:
    QueryPacket(char code, std::string message) : flag_(code) {
        message_.insert(message_.begin(), message.begin(), message.end());
    }

    QueryPacket() = default;

    [[maybe_unused]] void read(const char *data, unsigned length) override {};

    void read(std::vector<char> &) override;

    void write(std::vector<char> &buffer) const override;

    void show() const override;
};


#endif //PROTOOFMYSQL_QUERYPACKET_H

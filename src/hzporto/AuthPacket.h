//
// Created by wangyufan on 2024-01-15.
//

#ifndef PROTOOFMYSQL_AUTHPACKET_H
#define PROTOOFMYSQL_AUTHPACKET_H

#include "MySQLPacket.h"

class AuthPacket : public MySQLPacket {
    friend class HzMySQLClient;

private:
    const std::vector<char> FILLER_23{0};
private:
    long client_flags_;
    long max_package_size_;
    char charset_index_;
    std::vector<char> extra_;
    std::string user_;
    std::vector<char> password_;
    std::string database_;
public:
    void read(const char *data, unsigned length) override;

    void read(std::vector<char> &) override;

    void write(std::vector<char> &buffer) const override;

    [[nodiscard]] int calculatePacketSize() const override;

    [[nodiscard]] std::string getPacketInfo() const override;

    void show() const override;
};

#endif //PROTOOFMYSQL_AUTHPACKET_H

//
// Created by wangyufan on 2024-01-15.
//

#ifndef PROTOOFMYSQL_HANDSHAKEPACKET_H
#define PROTOOFMYSQL_HANDSHAKEPACKET_H

#include "MySQLPacket.h"
#include <vector>

class HandshakePacket : public MySQLPacket {

    friend class HzMySQLClient;

private:
    const char FILLER_13[13]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
private:
    char protocol_version_{};
    std::vector<char> server_version_;
    long thread_id_{};
    std::vector<char> seed_;
    int server_capabilities_{};
    char server_charset_index_{};
    int server_status_{};
    std::vector<char> rest_of_scramble_buff_;

    HandshakePacket() = default;

public:
    void read(const char *data, unsigned length) override;

    void read(std::vector<char> &) override;

    [[nodiscard]] int calculatePacketSize() const override;

    [[nodiscard]] std::string getPacketInfo() const override;

    void write(std::vector<char> &buffer) const override;

    void show() const override;
};


#endif //PROTOOFMYSQL_HANDSHAKEPACKET_H

//
// Created by wangyufan on 2024-01-21.
//

#ifndef PROTOOFMYSQL_ERRORPACKET_H
#define PROTOOFMYSQL_ERRORPACKET_H

#include "MySQLPacket.h"

class ErrorPacket : public MySQLPacket {
    friend class HzMySQLClient;
private:
    const char header_ = static_cast<char>(0xff);
    int errno_code_;
    char sql_state_marker_ = '#';
    std::vector<char> sql_state_{'H', 'Y', '0', '0', '0'};
    std::vector<char> error_message_;
private:
    [[nodiscard]]  int calculatePacketSize() const override;
protected:
    [[nodiscard]]  std::string getPacketInfo() const override;

public:
    [[maybe_unused]]  void read(const char *data, unsigned length)override {};

    void read(std::vector<char> &) override;

    void write(std::vector<char> &buffer) const override;

    void show() const override;
};


#endif //PROTOOFMYSQL_ERRORPACKET_H

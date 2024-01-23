//
// Created by wangyufan on 2024-01-21.
//

#ifndef PROTOOFMYSQL_QUITPACKET_H
#define PROTOOFMYSQL_QUITPACKET_H

#include "MySQLPacket.h"

class QuitPacket : public MySQLPacket {
    friend std::vector<char> productQuitPackage();

    friend class HzMySQLClientl;

private:
    std::vector<char> quit_;
protected:
    [[nodiscard]] std::string getPacketInfo() const override;

    [[nodiscard]] int calculatePacketSize() const override;

public:
    [[maybe_unused]] void read(const char *data, unsigned length) override {};

    void read(std::vector<char> &) override;

    void write(std::vector<char> &buffer) const override;

    void show() const override;
};

std::vector<char> productQuitPackage();

#endif //PROTOOFMYSQL_QUITPACKET_H

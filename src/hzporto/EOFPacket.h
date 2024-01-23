//
// Created by wangyufan on 2024-01-22.
//

#ifndef PROTOOFMYSQL_EOFPACKET_H
#define PROTOOFMYSQL_EOFPACKET_H

#include "MySQLPacket.h"

class EOFPacket : public MySQLPacket {
private:
    char header_ = static_cast<char>(0XFE);
    int warnings_{};
    int status_flags_{};

    [[nodiscard]]  int calculatePacketSize() const override;

protected:
    [[nodiscard]]  std::string getPacketInfo() const override;

public:
    [[nodiscard]] char getHeader()const;
    [[maybe_unused]] void read(const char *data, unsigned length) override {};

    void read(std::vector<char> &) override;

    void write(std::vector<char> &buffer) const override;

    void show() const override;
};


#endif //PROTOOFMYSQL_EOFPACKET_H

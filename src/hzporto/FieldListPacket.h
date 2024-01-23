//
// Created by wangyufan on 2024-01-22.
//

#ifndef PROTOOFMYSQL_FIELDLISTPACKET_H
#define PROTOOFMYSQL_FIELDLISTPACKET_H


#include "MySQLPacket.h"

class FieldListPacket : public MySQLPacket {
private:
    char flag_{};
    std::vector<char> table_;
    std::vector<char> filed_wild_card_;

    [[nodiscard]]  int calculatePacketSize() const override;

protected:
    [[nodiscard]]  std::string getPacketInfo() const override;

public:
    [[maybe_unused]] void read(const char *data, unsigned length) override {};

    void read(std::vector<char> &) override;

    void write(std::vector<char> &buffer) const override;

    void show() const override;
};


#endif //PROTOOFMYSQL_FIELDLISTPACKET_H

//
// Created by wangyufan on 2024-01-22.
//

#ifndef PROTOOFMYSQL_COLUMNCOUNTPACKET_H
#define PROTOOFMYSQL_COLUMNCOUNTPACKET_H

#include "MySQLPacket.h"

class ColumnCountPacket : public MySQLPacket {
private:
    int column_count_{};

    [[nodiscard]]  int calculatePacketSize() const override;

protected:
    [[nodiscard]]  std::string getPacketInfo() const override;

public:
    [[nodiscard]] int getColumnCount()const;
    [[maybe_unused]] void read(const char *data, unsigned length) override {};

    void read(std::vector<char> &) override;

    void write(std::vector<char> &buffer) const override;

    void show() const override;
};


#endif //PROTOOFMYSQL_COLUMNCOUNTPACKET_H

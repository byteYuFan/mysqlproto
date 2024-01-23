//
// Created by wangyufan on 2024-01-22.
//

#ifndef PROTOOFMYSQL_COLUMNDEFINITIONPACKET_H
#define PROTOOFMYSQL_COLUMNDEFINITIONPACKET_H

#include "MySQLPacket.h"

class ColumnDefinitionPacket : public MySQLPacket {
private:
    const std::vector<char> DEFAULT_CATALOG{'d', 'e', 'f'};
    const char NEXT_LENGTH = 0X0C;
    const std::vector<char> FILLER{0X00, 0X00};
    std::vector<char> catalog_ = DEFAULT_CATALOG;
    std::vector<char> schema_;
    std::vector<char> table_;
    std::vector<char> org_table_;
    std::vector<char> name_;
    std::vector<char> org_name_;
    char next_length_ = NEXT_LENGTH;
    int charset_set_{};
    long length_{};
    int type_{};
    int flags_{};
    char decimals_{};
    std::vector<char> filler_ = FILLER;
    std::vector<char> default_value_;

    [[nodiscard]]  int calculatePacketSize() const override;

protected:
    [[nodiscard]]  std::string getPacketInfo() const override;

public:
    [[maybe_unused]] void read(const char *data, unsigned length) override {};

    void read(std::vector<char> &) override;

    void write(std::vector<char> &buffer) const override;

    void show() const override;

public:
    std::vector<char> getColumnName();
};


#endif //PROTOOFMYSQL_COLUMNDEFINITIONPACKET_H

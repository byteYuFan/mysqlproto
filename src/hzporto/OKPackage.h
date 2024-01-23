//
// Created by wangyufan on 2024-01-21.
//

#ifndef PROTOOFMYSQL_OKPACKAGE_H
#define PROTOOFMYSQL_OKPACKAGE_H

#include "MySQLPacket.h"

class OKPackage : public MySQLPacket {
    friend class HzMySQLClient;
private:
    const static char HEADER = 0x00;
    const std::vector<char> OK = {7, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0};
    const std::vector<char> AC_OFF = {7, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
private:
    char header_ = HEADER;
    long affected_rows_;
    long insert_id_;
    int server_status_;
    int waring_count_;
    std::vector<char> message_;
protected:
    [[nodiscard]] std::string getPacketInfo() const override;
    [[nodiscard]] int calculatePacketSize() const override;

public:
    [[maybe_unused]] void read(const char *data, unsigned length) override{};

    void read(std::vector<char> &) override;

    void write(std::vector<char> &buffer) const override{};

    void show() const override;
public:
    [[nodiscard]] char getHeader()const;
};


#endif //PROTOOFMYSQL_OKPACKAGE_H

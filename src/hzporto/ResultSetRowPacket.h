//
// Created by wangyufan on 2024-01-22.
//

#ifndef PROTOOFMYSQL_RESULTSETROWPACKET_H
#define PROTOOFMYSQL_RESULTSETROWPACKET_H

#include "MySQLPacket.h"
#include <list>
#include <memory>

class ResultSetRowPacket : public MySQLPacket {
private:
    const char NULL_MASK = static_cast<char>(251);
    int column_count_{};
    std::shared_ptr<std::vector<std::shared_ptr<std::vector<char>>>> column_values_;
    // todo 记得释放资源

    [[nodiscard]]  int calculatePacketSize() const override;

protected:
    [[nodiscard]]  std::string getPacketInfo() const override;

public:
    ResultSetRowPacket() = default;

    explicit ResultSetRowPacket(int column_count) : column_count_(column_count) {
        column_values_ = std::make_shared<std::vector<std::shared_ptr<std::vector<char>>>>();
    }

    ~ResultSetRowPacket() = default;

    std::shared_ptr<std::vector<std::shared_ptr<std::vector<char>>>>  getColumnValues();

    [[maybe_unused]] void read(const char *data, unsigned length) override {};

    void read(std::vector<char> &) override;

    void write(std::vector<char> &buffer) const override;

    void show() const override;
};


#endif //PROTOOFMYSQL_RESULTSETROWPACKET_H

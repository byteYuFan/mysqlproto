//
// Created by wangyufan on 2024-01-15.
//

#ifndef PROTOOFMYSQL_MYSQLDATA_H
#define PROTOOFMYSQL_MYSQLDATA_H

#include <vector>
#include <string>
#include <ctime>
#include <cstring>
class MySQLData {
private:
    static const long NULL_LENGTH = -1;
    const std::vector<char> EMPTY_BYTES{};
public:
    explicit MySQLData(const std::vector<char>& data) : data_(data), length_(static_cast<int>(data.size())), position_(0) {}

    ~MySQLData()=default;
public:
    [[nodiscard]] unsigned long length()const;
    [[nodiscard]] int position()const;
    [[nodiscard]] std::vector<char> data()const;
    void move(int i);
    void setPosition(int i);
    [[nodiscard]] bool haveRemaining() const;
    char read(int i);
    char read();
    int readUInt16();
    int readUInt24();
    long readUInt32();
    int readInt();
    long readLong();
    long readLength();
    std::vector<char> readBytes();
    std::vector<char> readBytes(int length);
    std::vector<char> readBytesWithNull();
    std::vector<char> readBytesWithLength();
    std::string readString();
    std::string readStringWithNull();
    std::string readStringWithLength();
    std::vector<char> readStringWithLengthVector();
    std::tm readTime();
    std::tm readDate();

private:
    const std::vector<char> data_ ;
    int length_;
    int position_;
};

// 静态常量初始化



#endif //PROTOOFMYSQL_MYSQLDATA_H

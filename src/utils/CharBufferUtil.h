//
// Created by wangyufan on 2024-01-15.
//

#ifndef PROTOOFMYSQL_CHARBUFFERUTIL_H
#define PROTOOFMYSQL_CHARBUFFERUTIL_H


#include <iostream>
#include <vector>
#include <cstring>

class CharBufferUtil {
public:
    static void writeUInt16(std::vector<char>& buffer, int i);
    static void writeUInt24(std::vector<char>& buffer, int i);
    static void writeInt(std::vector<char>& buffer, int i);
    static void writeFloat(std::vector<char>& buffer, float f);
    static void writeUInt32(std::vector<char>& buffer, long l);
    static void writeLong(std::vector<char>& buffer, long l);
    static void writeDouble(std::vector<char>& buffer, double d);
    static void writeLength(std::vector<char>& buffer, long l);
    static void writeWithNull(std::vector<char>& buffer, const char* src);
    static void writeWithNull(std::vector<char>& buffer, const std::string &str);
    static void writeWithNull(std::vector<char>& buffer, const std::vector<char> &str);
    static void writeWithLength(std::vector<char>& buffer, const char* src);
    static void writeWithLength(std::vector<char>& buffer, const std::vector<char>& src);
    static void writeWithLength(std::vector<char>& buffer, const std::string &str);
    static void writeWithLength(std::vector<char>& buffer, const char* src, char nullValue);
    static int getLength(long length);
    static int getLength(std::vector<char>src);
    static int getLength(const char* src);
};


#endif //PROTOOFMYSQL_CHARBUFFERUTIL_H

//
// Created by wangyufan on 2024-01-22.
//

#ifndef PROTOOFMYSQL_BYTEUTIL_H
#define PROTOOFMYSQL_BYTEUTIL_H

#include "../hzporto/MySQLPacket.h"

class ByteUtil {

public:
    static void bytesCut(MySQLPacket &mysqlPacket, std::vector<char> &bytes);
    static void bytesCut(MySQLPacket *mysqlPacket, std::vector<char> &bytes);
};


#endif //PROTOOFMYSQL_BYTEUTIL_H

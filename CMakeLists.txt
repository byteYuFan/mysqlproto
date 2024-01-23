cmake_minimum_required(VERSION 3.10.2)
project(protoofmysql)

set(CMAKE_CXX_STANDARD 17)


add_executable(protoofmysql
        src/protocol.cpp
        src/hzporto/hz_protocol.h
        src/hzporto/MySQLData.cpp
        src/hzporto/MySQLData.h
        src/hzporto/MySQLPacket.cpp
        src/hzporto/MySQLPacket.h
        src/hzporto/HandshakePacket.cpp
        src/hzporto/HandshakePacket.h
        src/utils/CharBufferUtil.cpp
        src/utils/CharBufferUtil.h
        src/utils/SecurityUtil.cpp
        src/utils/SecurityUtil.h
        src/hzporto/AuthPacket.cpp
        src/hzporto/AuthPacket.h
        src/utils/sha.hpp
        src/hzporto/MySQLData.cpp
        src/hzporto/hzmysql.h
        src/hzporto/hzmysql.h
        src/hzporto/OKPackage.cpp
        src/hzporto/OKPackage.h
        src/hzporto/ErrorPacket.cpp
        src/hzporto/ErrorPacket.h
        src/hzporto/QuitPacket.cpp
        src/hzporto/QuitPacket.h
        src/hzporto/hzmysql.cpp
        src/HzMySQLClient.cpp
        src/HzMySQLClient.h
        src/hzporto/QueryPacket.cpp
        src/hzporto/QueryPacket.h
        src/hzporto/EOFPacket.cpp
        src/hzporto/EOFPacket.h
        src/hzporto/ColumnCountPacket.cpp
        src/hzporto/ColumnCountPacket.h
        src/hzporto/ColumnDefinitionPacket.cpp
        src/hzporto/ColumnDefinitionPacket.h
        src/hzporto/FieldListPacket.cpp
        src/hzporto/FieldListPacket.h
        src/hzporto/ResultsetRowPacket.cpp
        src/hzporto/ResultSetRowPacket.h
        src/utils/ByteUtil.cpp
        src/utils/ByteUtil.h
        src/hzporto/deal_command.cpp
        src/hzporto/deal_command.h
)
target_link_libraries(protoofmysql -lssl -lcrypto)

//
// Created by wangyufan on 2024-01-21.
//

#include "ErrorPacket.h"
#include "MySQLData.h"
#include <iostream>
int ErrorPacket::calculatePacketSize() const {
    int size = 9;
    if (!error_message_.empty())
        size += static_cast<int>(error_message_.size());

    return size;
}

std::string ErrorPacket::getPacketInfo() const {
    return "MySQL Error Packet";
}

void ErrorPacket::read(std::vector<char> &src) {
    MySQLData md(src);
    packet_length_ = md.readUInt24();
    packet_id_ = md.read();
    md.read();
    errno_code_ = md.readUInt16();
    if (md.haveRemaining() && (md.read(md.position()) == (static_cast<char>('#')))) {
        md.read();
        sql_state_ = md.readBytes(5);
    }
    error_message_ = md.readBytes();

}

void ErrorPacket::write(std::vector<char> &buffer) const {

}

void ErrorPacket::show() const {


    std::cout << "Header: " << static_cast<int>(header_) << std::endl;
    std::cout << "Error Number: " << errno_code_ << std::endl;

    std::cout << "SQL State: " << sql_state_marker_;
    for (const char &stateChar : sql_state_) {
        std::cout << stateChar;
    }
    std::cout << std::endl;

    std::cout << "Error Message: ";
    for (const char &byte : error_message_) {
        printf("%c",byte);
    }
    std::cout << std::endl;
}
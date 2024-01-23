//
// Created by wangyufan on 2024-01-21.
//

#include "OKPackage.h"
#include "../utils/CharBufferUtil.h"
#include "MySQLData.h"

std::string OKPackage::getPacketInfo() const {
    return "MySQL OK Packet";
}

int OKPackage::calculatePacketSize() const {
    int size = 1;
    size += CharBufferUtil::getLength(affected_rows_);
    size += CharBufferUtil::getLength(insert_id_);
    size += 4;
    if (!message_.empty())
        size += CharBufferUtil::getLength(message_);

    return size;
}

void OKPackage::read(std::vector<char> &data) {
    MySQLData md(data);
    packet_length_ = md.readUInt24();
    packet_id_ = md.read();
    header_ = md.read();
    affected_rows_ = md.readLength();
    insert_id_ = md.readLength();
    server_status_ = md.readUInt16();
    waring_count_ = md.readUInt16();
    if (md.haveRemaining())
        message_ = md.readBytesWithLength();
}

void OKPackage::show() const {
    std::cout << "Header: " << static_cast<int>(header_) << std::endl;
    std::cout << "Affected Rows: " << affected_rows_ << std::endl;
    std::cout << "Insert ID: " << insert_id_ << std::endl;
    std::cout << "Server Status: " << server_status_ << std::endl;
    std::cout << "Warning Count: " << waring_count_ << std::endl;

    std::cout << "Message: ";
    for (const char &byte: message_) {
        printf("%c", byte);
    }
    std::cout << std::endl;
}

char OKPackage::getHeader() const {
    return header_;
}

//
// Created by wangyufan on 2024-01-22.
//

#include "FieldListPacket.h"
#include "MySQLData.h"

int FieldListPacket::calculatePacketSize() const {
    int size = 1;
    size += static_cast<int>( table_.size()) + 1;
    size += static_cast<int>(filed_wild_card_.size());
    return size;
}

std::string FieldListPacket::getPacketInfo() const {
    return "MySQL Field List Packet.";
}

void FieldListPacket::read(std::vector<char> &src) {
    MySQLData md(src);
    packet_length_ = md.readUInt24();
    packet_id_ = md.read();
    flag_ = md.read();
    table_ = md.readBytesWithNull();
    filed_wild_card_ = md.readBytes();
}

void FieldListPacket::write(std::vector<char> &buffer) const {

}

void FieldListPacket::show() const {
    MySQLPacket::show();
}

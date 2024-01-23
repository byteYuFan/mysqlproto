//
// Created by wangyufan on 2024-01-22.
//

#include "EOFPacket.h"
#include "MySQLData.h"

int EOFPacket::calculatePacketSize() const {
    return 1 + 2 + 2;
}

std::string EOFPacket::getPacketInfo() const {
    return "MySQL EOF Packet.";
}

void EOFPacket::read(std::vector<char> &src) {
    MySQLData md(src);
    packet_length_ = md.readUInt24();
    packet_id_ = md.read();
    header_ = md.read();
    warnings_ = md.readUInt16();
    status_flags_ = md.readUInt16();
}

void EOFPacket::write(std::vector<char> &buffer) const {

}

void EOFPacket::show() const {
    MySQLPacket::show();
}

char EOFPacket::getHeader() const {
    return header_;
}

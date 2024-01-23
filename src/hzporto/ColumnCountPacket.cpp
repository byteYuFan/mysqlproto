//
// Created by wangyufan on 2024-01-22.
//

#include "ColumnCountPacket.h"
#include "../utils/CharBufferUtil.h"
#include "MySQLData.h"

int ColumnCountPacket::calculatePacketSize() const {
    return static_cast<int>(CharBufferUtil::getLength(column_count_));
}

std::string ColumnCountPacket::getPacketInfo() const {
    return "MySQL ColumnCount Packet";
}

void ColumnCountPacket::read(std::vector<char> &src) {
    MySQLData md(src);
    packet_length_ = md.readUInt24();
    packet_id_ = md.read();
    column_count_ = static_cast<int>(md.readLength());
}

void ColumnCountPacket::write(std::vector<char> &buffer) const {

}

void ColumnCountPacket::show() const {
    MySQLPacket::show();
}

int ColumnCountPacket::getColumnCount() const {
    return column_count_;
}

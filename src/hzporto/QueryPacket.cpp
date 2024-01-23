//
// Created by wangyufan on 2024-01-22.
//

#include "QueryPacket.h"
#include "../utils/CharBufferUtil.h"

int QueryPacket::calculatePacketSize() const {
    int size = 1;
    if (!message_.empty())
        size += static_cast<int>(message_.size());
    return size;
}

std::string QueryPacket::getPacketInfo() const {
    return "MySQL Query Packet.";
}

void QueryPacket::read(std::vector<char> &) {

}

void QueryPacket::write(std::vector<char> &buffer) const {
    int size = calculatePacketSize();
    CharBufferUtil::writeUInt24(buffer, size);
    buffer.push_back(packet_id_);
    buffer.push_back(COM_QUERY);
    buffer.insert(buffer.end(), message_.begin(), message_.end());
}

void QueryPacket::show() const {
    MySQLPacket::show();
}

//
// Created by wangyufan on 2024-01-21.
//

#include "QuitPacket.h"
#include "../utils/CharBufferUtil.h"

std::string QuitPacket::getPacketInfo() const {
    return "MySQL Quit Packet";;
}

int QuitPacket::calculatePacketSize() const {
    return 1;
}

void QuitPacket::read(std::vector<char> &) {

}

void QuitPacket::show() const {
    MySQLPacket::show();
}

void QuitPacket::write(std::vector<char> &buffer) const {
    CharBufferUtil::writeUInt24(buffer, calculatePacketSize());
    buffer.push_back(packet_id_);
    buffer.push_back(COM_QUIT);
}

std::vector<char> productQuitPackage() {

}

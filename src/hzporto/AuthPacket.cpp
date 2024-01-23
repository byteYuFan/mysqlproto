//
// Created by wangyufan on 2024-01-15.
//

#include "AuthPacket.h"
#include "MySQLData.h"
#include "hz_protocol.h"
#include "../utils/CharBufferUtil.h"
#include "../utils/SecurityUtil.h"

void AuthPacket::read(const char *data, unsigned int length) {
    auto auth_data = std::vector<char>(data, data + length);
    MySQLData md(auth_data);
    packet_length_ = md.readUInt24();
    packet_id_ = md.read();
    client_flags_ = md.readUInt32();
    max_package_size_ = md.readUInt32();
    charset_index_ = md.read();
    int current = md.position();
    int len = static_cast<int>(md.readLength());
    if (len > 0 && len < static_cast<int>(FILLER_23.size())) {
        auto *ab = new std::vector<char>(len);
        ab->insert(ab->begin(), md.data().begin() + md.position(), md.data().begin() + md.position() + len);
        extra_ = *ab;
        delete ab;
    }
    md.setPosition(current + FILLER_23.size());
    user_ = md.readStringWithNull();
    password_.clear();
    auto p = md.readStringWithLengthVector();
    password_.insert(password_.begin(), p.begin(), p.end());
    if (((client_flags_ & Capabilities::CLIENT_CONNECT_WITH_DB) != 0) && md.haveRemaining()) {
        database_ = md.readStringWithNull();
    }
}

void AuthPacket::write(std::vector<char> &buffer) const {
    CharBufferUtil::writeUInt24(buffer, calculatePacketSize());
    buffer.push_back(packet_id_);
    CharBufferUtil::writeUInt32(buffer, client_flags_);
    CharBufferUtil::writeUInt32(buffer, max_package_size_);
    buffer.push_back(static_cast<char>(charset_index_));
    for (int i = 0; i < 23; i++)buffer.push_back(FILLER_23[i]);
    if (user_.empty())
        buffer.push_back(static_cast<char>(0));
    else
        CharBufferUtil::writeWithNull(buffer, user_);

    if (password_.empty())
        buffer.push_back(static_cast<char>(0));
    else
        CharBufferUtil::writeWithLength(buffer, password_);

    if (database_.empty())
        buffer.push_back(static_cast<char>(0));
    else
        CharBufferUtil::writeWithNull(buffer, database_);
}

int AuthPacket::calculatePacketSize() const {
    int size = 32;
    size += user_.empty() ? 1 : static_cast<int>(user_.size()) + 1;
    size += password_.empty() ? 1 : static_cast<int>(password_.size()) + 1;
    size += database_.empty() ? 1 : static_cast<int>(database_.size()) + 1;
    return size;
}

std::string AuthPacket::getPacketInfo() const {
    return "MySQL Authentication Packet";
}

void AuthPacket::read(std::vector<char> &auth_data) {
    MySQLData md(auth_data);
    packet_length_ = md.readUInt24();
    packet_id_ = md.read();
    client_flags_ = md.readUInt32();
    max_package_size_ = md.readUInt32();
    charset_index_ = md.read();
    int current = md.position();
    int len = static_cast<int>(md.readLength());
    if (len > 0 && len < static_cast<int>( FILLER_23.size())) {
        auto *ab = new std::vector<char>(len);
        ab->insert(ab->begin(), md.data().begin() + md.position(), md.data().begin() + md.position() + len);
        extra_ = *ab;
        delete ab;
    }
    md.setPosition(current + FILLER_23.size());
    user_ = md.readStringWithNull();
    password_.clear();
    auto p = md.readStringWithLengthVector();
    password_.insert(password_.begin(), p.begin(), p.end());
    if (((client_flags_ & Capabilities::CLIENT_CONNECT_WITH_DB) != 0) && md.haveRemaining()) {
        database_ = md.readStringWithNull();
    }
}

void AuthPacket::show() const {
    MySQLPacket::show();
}

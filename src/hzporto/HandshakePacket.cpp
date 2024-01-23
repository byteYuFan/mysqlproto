//
// Created by wangyufan on 2024-01-15.
//

#include "HandshakePacket.h"
#include "../utils/CharBufferUtil.h"
#include "MySQLData.h"
#include <cstdio>

void HandshakePacket::read(const char *data, unsigned len) {
    auto vc_data = std::vector<char>(data, data + len);
    MySQLData md(vc_data);
    packet_length_ = md.readUInt24();
    packet_id_ = md.read();
    protocol_version_ = md.read();
    server_version_ = md.readBytesWithNull();
    thread_id_ = md.readUInt32();
    seed_ = md.readBytesWithNull();
    server_capabilities_ = md.readUInt16();
    server_charset_index_ = md.read();
    server_status_ = md.readUInt16();
    md.move(13);
    rest_of_scramble_buff_ = md.readBytesWithNull();
}

void HandshakePacket::read(std::vector<char> &vc_data) {
    MySQLData md(vc_data);
    packet_length_ = md.readUInt24();
    packet_id_ = md.read();
    protocol_version_ = md.read();
    server_version_ = md.readBytesWithNull();
    thread_id_ = md.readUInt32();
    seed_ = md.readBytesWithNull();
    server_capabilities_ = md.readUInt16();
    server_charset_index_ = md.read();
    server_status_ = md.readUInt16();
    md.move(13);
    rest_of_scramble_buff_ = md.readBytesWithNull();
}

int HandshakePacket::calculatePacketSize() const {
    int size = 1;
    size += static_cast<int>(server_version_.size());
    size += 4;
    size += static_cast<int>(seed_.size());
    size += 1;
    size += 2;
    size += 1;
    size += 1;
    size += 10;
    size += static_cast<int>(rest_of_scramble_buff_.size());
    size += 1;
    size += 4;
    size += 1;
    return size;
}

void HandshakePacket::write(std::vector<char> &buffer) const {
    CharBufferUtil::writeUInt24(buffer, calculatePacketSize());
    buffer.push_back(packet_id_);
    buffer.push_back(protocol_version_);
    CharBufferUtil::writeWithNull(buffer, server_version_);
    CharBufferUtil::writeUInt32(buffer, thread_id_);
    CharBufferUtil::writeWithNull(buffer, seed_);
    CharBufferUtil::writeUInt16(buffer, server_capabilities_);
    buffer.push_back(server_charset_index_);
    CharBufferUtil::writeUInt16(buffer, server_status_);
    for (char i: FILLER_13)buffer.push_back(i);
    CharBufferUtil::writeWithNull(buffer, rest_of_scramble_buff_);
}

std::string HandshakePacket::getPacketInfo() const {
    return "MySQL Handshake Packet";
}

void HandshakePacket::show() const {
    std::cout << "package length:" << packet_length_ << std::endl;
    printf("package id :%d\n", packet_id_);
    printf("protocol Version:%d\n", protocol_version_);
    std::cout << "Server Version: " << std::string(server_version_.begin(), server_version_.end()) << std::endl;
    std::cout << "Thread ID: " << thread_id_ << std::endl;
    std::cout << "Seed: " << std::string(seed_.begin(), seed_.end()) << std::endl;
    std::cout << "Server Capabilities: " << server_capabilities_ << std::endl;
    std::cout << "Server Charset Index: " << static_cast<int>(server_charset_index_) << std::endl;
    std::cout << "Server Status: " << server_status_ << std::endl;
    std::cout << "Rest of Scramble Buff: " << std::string(rest_of_scramble_buff_.begin(), rest_of_scramble_buff_.end())
              << std::endl;

}



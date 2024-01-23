//
// Created by wangyufan on 2024-01-22.
//

#include "ResultSetRowPacket.h"
#include "../utils/CharBufferUtil.h"
#include "MySQLData.h"

int ResultSetRowPacket::calculatePacketSize() const {
    int size = 0;
    for (int i = 0; i < column_count_; i++) {
        auto v = column_values_->at(i);
        size += (v->empty()) ? 1 : CharBufferUtil::getLength(*v);
    }
    return size;
}

std::string ResultSetRowPacket::getPacketInfo() const {
    return "MySQL Result Set Row Packet";
}

void ResultSetRowPacket::read(std::vector<char> &src) {
    MySQLData md(src);
    packet_length_ = md.readUInt24();
    packet_id_ = md.read();
    for (int i = 0; i < column_count_; i++) {
        auto v = md.readBytesWithLength();
        auto in = std::make_shared<std::vector<char>>(v.begin(), v.end());
        column_values_->push_back(in);
    }
}

void ResultSetRowPacket::write(std::vector<char> &buffer) const {

}

void ResultSetRowPacket::show() const {
    MySQLPacket::show();
}

std::shared_ptr<std::vector<std::shared_ptr<std::vector<char>>>>  ResultSetRowPacket::getColumnValues() {
    return column_values_;
}

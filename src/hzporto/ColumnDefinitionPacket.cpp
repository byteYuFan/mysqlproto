//
// Created by wangyufan on 2024-01-22.
//

#include "ColumnDefinitionPacket.h"
#include "../utils/CharBufferUtil.h"
#include "MySQLData.h"

int ColumnDefinitionPacket::calculatePacketSize() const {
    int size = 0;
    size += (catalog_.empty() ? 1 : CharBufferUtil::getLength(catalog_));
    size += (schema_.empty() ? 1 : CharBufferUtil::getLength(schema_));
    size += (table_.empty() ? 1 : CharBufferUtil::getLength(table_));
    size += (org_table_.empty() ? 1 : CharBufferUtil::getLength(org_table_));
    size += (name_.empty() ? 1 : CharBufferUtil::getLength(schema_));
    size += (org_name_.empty() ? 1 : CharBufferUtil::getLength(org_name_));
    size += 13;
    if (!default_value_.empty())
        size += CharBufferUtil::getLength(default_value_);
    return size;
}

std::string ColumnDefinitionPacket::getPacketInfo() const {
    return "MySQL Column Definition Packet";
}

void ColumnDefinitionPacket::read(std::vector<char> &src) {
    MySQLData md(src);
    packet_length_ = md.readUInt24();
    packet_id_ = md.read();
    catalog_ = md.readBytesWithLength();
    schema_ = md.readBytesWithLength();
    table_ = md.readBytesWithLength();
    org_table_ = md.readBytesWithLength();
    name_ = md.readBytesWithLength();
    org_name_ = md.readBytesWithLength();
    next_length_ = md.read();
    charset_set_ = md.readUInt16();
    length_ = md.readUInt32();
    type_ = md.read() & 0xff;
    flags_ = md.readUInt16();
    decimals_ = md.read();
    filler_ = md.readBytes(2);
    if (md.haveRemaining())
        default_value_ = md.readBytesWithLength();
}

void ColumnDefinitionPacket::write(std::vector<char> &buffer) const {

}

void ColumnDefinitionPacket::show() const {
    MySQLPacket::show();
}

std::vector<char> ColumnDefinitionPacket::getColumnName() {
    return name_;
}

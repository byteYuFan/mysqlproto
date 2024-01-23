//
// Created by wangyufan on 2024-01-15.
//

#include <algorithm>
#include "MySQLData.h"
#include <cstring>
unsigned long MySQLData::length() const {
    return length_;
}

int MySQLData::position() const {
    return position_;
}


std::vector<char> MySQLData::data() const {
    return data_;
}

void MySQLData::move(int i) {
    position_ += i;
}

void MySQLData::setPosition(int i) {
    position_ = i;
}

bool MySQLData::haveRemaining() const {
    return length_ > position_;
}

char MySQLData::read(int i) {
    return data_[i];
}

char MySQLData::read() {
    return data_[position_++];
}

int MySQLData::readUInt16() {
    std::vector<char> d = data_;
    int i = d[position_++] & 0xff;
    i |= (d[position_++] & 0xff) << 8;
    return i;
}

int MySQLData::readUInt24() {
    std::vector<char> d = data_;
    int i = d[position_++] & 0xff;
    i |= (d[position_++] & 0xff) << 8;
    i |= (d[position_++] & 0xff) << 16;
    return i;
}

long MySQLData::readUInt32() {
    std::vector<char> d = data_;
    long i = static_cast<long>(d[position_++] & 0xff);
    i |= static_cast<long>(d[position_++] & 0xff) << 8;
    i |= static_cast<long>(d[position_++] & 0xff) << 16;
    i |= static_cast<long>(d[position_++] & 0xff) << 24;
    return i;
}

int MySQLData::readInt() {
    std::vector<char> d = data_;
    int i = d[position_++] & 0xff;
    i |= (d[position_++] & 0xff) << 8;
    i |= (d[position_++] & 0xff) << 16;
    i |= (d[position_++] & 0xff) << 24;
    return i;
}

long MySQLData::readLong() {
    std::vector<char> d = data_;
    long l = static_cast<long>(d[position_++] & 0xff);
    l |= static_cast<long>(d[position_++] & 0xff) << 8;
    l |= static_cast<long>(d[position_++] & 0xff) << 16;
    l |= static_cast<long>(d[position_++] & 0xff) << 24;
    l |= static_cast<long>(d[position_++] & 0xff) << 32;
    l |= static_cast<long>(d[position_++] & 0xff) << 40;
    l |= static_cast<long>(d[position_++] & 0xff) << 48;
    l |= static_cast<long>(d[position_++] & 0xff) << 56;
    return l;
}

long MySQLData::readLength() {
    int length = data_[position_++] & 0xff;
    switch (length) {
        case 251:
            return NULL_LENGTH;
        case 252:
            return readUInt16();
        case 253:
            return readUInt24();
        case 254:
            return readLong();
        default:
            return length;
    }
}

std::vector<char> MySQLData::readBytes() {
    if (position_ >= length_) {
        return EMPTY_BYTES;
    }
    std::vector<char> result(data_.begin() + position_, data_.end());
    position_ = length_;
    return result;
}

std::vector<char> MySQLData::readBytes(int length) {
    std::vector<char> result(data_.begin() + position_, data_.begin() + position_ + length);
    position_ += length;
    return result;
}

std::vector<char> MySQLData::readBytesWithNull() {
    if (position_ >= length_) {
        return EMPTY_BYTES;
    }

    auto it = std::find(data_.begin() + position_, data_.end(), '\0');
    if (it != data_.end()) {
        int offset = static_cast<int>(std::distance(data_.begin(), it));
        std::vector<char> result(data_.begin() + position_, it);
        position_ = offset + 1;
        return result;
    } else {
        std::vector<char> result(data_.begin() + position_, data_.end());
        position_ = length_;
        return result;
    }
}

std::vector<char> MySQLData::readBytesWithLength() {
    int length = static_cast<int>(readLength());
    if (length == NULL_LENGTH) {
        return EMPTY_BYTES;
    }
    if (length <= 0) {
        return EMPTY_BYTES;
    }
    std::vector<char> result(data_.begin() + position_, data_.begin() + position_ + length);
    position_ += length;
    return result;
}

std::string MySQLData::readString() {
    if (position_ >= length_) {
        return "";
    }
    std::string result(data_.begin() + position_, data_.end());
    position_ = length_;
    return result;
}

std::string MySQLData::readStringWithNull() {
    if (position_ >= length_) {
        return "";
    }
    auto it = std::find(data_.begin() + position_, data_.end(), '\0');
    if (it != data_.end()) {
        int offset = static_cast<int>(std::distance(data_.begin(), it));
        std::string result(data_.begin() + position_, it);
        position_ = offset + 1;
        return result;
    } else {
        std::string result(data_.begin() + position_, data_.end());
        position_ = length_;
        return result;
    }
}

std::string MySQLData::readStringWithLength() {
    int length = static_cast<int>(readLength());
    if (length <= 0) {
        return "";
    }
    std::string result(data_.begin() + position_, data_.begin() + position_ + length);
    position_ += length;
    return result;
}
std::vector<char> MySQLData::readStringWithLengthVector() {
    int length = static_cast<int>(readLength());
    if (length <= 0) {
        return {};
    }
    std::vector<char> result(data_.begin() + position_, data_.begin() + position_ + length);
    position_ += length;
    return result;
}
std::tm MySQLData::readTime() {
    move(6);
    int hour = static_cast<unsigned char>(read());
    int minute = static_cast<unsigned char>(read());
    int second = static_cast<unsigned char>(read());
    std::tm time = {};
    time.tm_hour = hour;
    time.tm_min = minute;
    time.tm_sec = second;
    return time;
}

std::tm MySQLData::readDate() {
    char length = read();
    int year = readUInt16();
    char month = read();
    int day = static_cast<unsigned char>(read());
    int hour = static_cast<unsigned char>(read());
    int minute = static_cast<unsigned char>(read());
    int second = static_cast<unsigned char>(read());
    if (length == 11) {
        long nanos = readUInt32();
        std::tm time = {};
        time.tm_year = year - 1900;
        time.tm_mon = month - 1;
        time.tm_mday = day;
        time.tm_hour = hour;
        time.tm_min = minute;
        time.tm_sec = second;
        // 处理纳秒
        // 注意：这里只是一个简单的示例，实际情况可能需要更复杂的处理
        int64_t nanoseconds = nanos / 1000000;
        time.tm_sec = static_cast<int>(nanoseconds % 1000000);
        return time;
    } else {
        std::tm date = {};
        date.tm_year = year - 1900;
        date.tm_mon = month - 1;
        date.tm_mday = day;
        date.tm_hour = hour;
        date.tm_min = minute;
        date.tm_sec = second;
        return date;
    }
}


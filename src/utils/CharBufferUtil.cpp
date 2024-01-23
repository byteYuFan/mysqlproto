//
// Created by wangyufan on 2024-01-15.
//

#include "CharBufferUtil.h"

void CharBufferUtil::writeUInt16(std::vector<char> &buffer, int i) {
    buffer.push_back(static_cast<char>(i & 0xff));
    buffer.push_back(static_cast<char>(i >> 8));
}

void CharBufferUtil::writeUInt24(std::vector<char> &buffer, int i) {
    buffer.push_back(static_cast<char>(i & 0xff));
    buffer.push_back(static_cast<char>(i >> 8));
    buffer.push_back(static_cast<char>(i >> 16));
}

void CharBufferUtil::writeInt(std::vector<char> &buffer, int i) {
    buffer.push_back(static_cast<char>(i & 0xff));
    buffer.push_back(static_cast<char>(i >> 8));
    buffer.push_back(static_cast<char>(i >> 16));
    buffer.push_back(static_cast<char>(i >> 24));
}

void CharBufferUtil::writeFloat(std::vector<char> &buffer, float f) {
    writeInt(buffer, *reinterpret_cast<int *>(&f));
}

void CharBufferUtil::writeUInt32(std::vector<char> &buffer, long l) {
    buffer.push_back(static_cast<char>(l & 0xff));
    buffer.push_back(static_cast<char>(l >> 8 & 0xff));
    buffer.push_back(static_cast<char>(l >> 16 & 0xff));
    buffer.push_back(static_cast<char>(l >> 24 & 0xff));
}

void CharBufferUtil::writeLong(std::vector<char> &buffer, long l) {
    for (int i = 0; i < 8; ++i) {
        buffer.push_back(static_cast<char>(l >> (i * 8)));
    }
}

void CharBufferUtil::writeDouble(std::vector<char> &buffer, double d) {
    writeLong(buffer, *reinterpret_cast<long *>(&d));
}

void CharBufferUtil::writeLength(std::vector<char> &buffer, long l) {
    if (l < 251) {
        buffer.push_back(static_cast<char>(l));
    } else if (l < 0x10000L) {
        buffer.push_back(static_cast<char>(252));
        writeUInt16(buffer, static_cast<int>(l));
    } else if (l < 0x1000000L) {
        buffer.push_back(static_cast<char>(253));
        writeUInt24(buffer, static_cast<int>(l));
    } else {
        buffer.push_back(static_cast<char>(254));
        writeLong(buffer, l);
    }
}

void CharBufferUtil::writeWithNull(std::vector<char> &buffer, const char *src) {
    buffer.insert(buffer.end(), src, src + strlen(src));
    buffer.push_back(static_cast<char>(0));
}

void CharBufferUtil::writeWithLength(std::vector<char> &buffer, const char *src) {
    int length = static_cast<int>( strlen(src));
    writeLength(buffer, length);
    buffer.insert(buffer.end(), src, src + length);
}

void CharBufferUtil::writeWithLength(std::vector<char> &buffer, const std::vector<char> &src) {
    int length = static_cast<int>(src.size());
    writeLength(buffer, length);
    buffer.insert(buffer.end(), src.begin(), src.end());
}

void CharBufferUtil::writeWithLength(std::vector<char> &buffer, const std::string &str) {
    writeLength(buffer, static_cast<long>(str.size()));
    buffer.insert(buffer.end(), str.begin(), str.end());
}

void CharBufferUtil::writeWithLength(std::vector<char> &buffer, const char *src, char nullValue) {
    if (src == nullptr) {
        buffer.push_back(nullValue);
    } else {
        writeWithLength(buffer, src);
    }
}

int CharBufferUtil::getLength(long length) {
    if (length < 251) {
        return 1;
    } else if (length < 0x10000L) {
        return 3;
    } else if (length < 0x1000000L) {
        return 4;
    } else {
        return 9;
    }
}

int CharBufferUtil::getLength(std::vector<char> src) {
    int length = static_cast<int>(src.size());
    if (length < 251) {
        return 1;
    } else if (length < 0x10000L) {
        return 3;
    } else if (length < 0x1000000L) {
        return 4;
    } else {
        return 9;
    }
}


int CharBufferUtil::getLength(const char *src) {
    int length = static_cast<int>( strlen(src));
    if (length < 251) {
        return 1 + length;
    } else if (length < 0x10000L) {
        return 3 + length;
    } else if (length < 0x1000000L) {
        return 4 + length;
    } else {
        return 9 + length;
    }
}

void CharBufferUtil::writeWithNull(std::vector<char> &buffer, const std::vector<char> &str) {
    buffer.insert(buffer.end(), str.begin(), str.end());
    buffer.push_back(static_cast<char>(0));
}

void CharBufferUtil::writeWithNull(std::vector<char> &buffer, const std::string &str) {
    buffer.insert(buffer.end(), str.begin(), str.end());
    buffer.push_back(static_cast<char>(0));
}



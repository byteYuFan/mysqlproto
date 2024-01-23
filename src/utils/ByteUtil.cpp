//
// Created by wangyufan on 2024-01-22.
//

#include <cstring>
#include "ByteUtil.h"

void ByteUtil::bytesCut(MySQLPacket &mysqlPacket, std::vector<char> &bytes) {
    size_t srcOffset = mysqlPacket.packet_length_ + 4;
    size_t destOffset = 0;
    size_t copyLength = bytes.size() - srcOffset;

    std::memcpy(bytes.data() + destOffset, bytes.data() + srcOffset, copyLength);
    // 如果需要缩小 vector 大小，可以使用 resize
    bytes.resize(copyLength);
}

void ByteUtil::bytesCut(MySQLPacket *mysqlPacket, std::vector<char> &bytes) {
    int srcOffset = mysqlPacket->packet_length_ + 4;
    // Check if srcOffset is within the bounds of the vector
    if (srcOffset >= 0 && srcOffset < bytes.size()) {
        // Calculate the destination offset and copy length
        int destOffset = 0;
        int copyLength = static_cast<int>(bytes.size()) - srcOffset;

        // Copy bytes from srcOffset to the end of the vector
        for (int i = 0; i < copyLength; ++i) {
            bytes[destOffset + i] = bytes[srcOffset + i];
        }

        // Resize the vector to remove the cut bytes
        bytes.resize(copyLength);
    }
    // If srcOffset is out of bounds, you may want to handle this case accordingly
}


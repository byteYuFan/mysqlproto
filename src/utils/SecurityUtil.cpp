//
// Created by wangyufan on 2024-01-15.
//

#include "SecurityUtil.h"
#include "sha.hpp"


static int hexCharToDecimal(char hexChar) {
    if (std::isdigit(hexChar)) {
        return hexChar - '0';
    } else {
        char lowerChar = std::tolower(hexChar);
        return 10 + (lowerChar - 'a');
    }
}

static std::vector<char> convertToCharArray(const std::string &inputString) {
    std::vector<char> charArray; // 清空现有的内容

    for (size_t i = 0; i < inputString.size(); i += 2) {
        char currentChar = 0;

        // 将两个十六进制数字合并成一个 char
        if (i < inputString.size()) {
            currentChar = hexCharToDecimal(inputString[i]) << 4;
        }

        if (i + 1 < inputString.size()) {
            currentChar |= hexCharToDecimal(inputString[i + 1]);
        }

        // 将合并后的 char 存入数组
        charArray.push_back(currentChar);
    }
    return charArray;
}

std::vector<char> scramble411(const std::string &password, const std::string &seed) {
    hz::SHA1 sha1, sha3;
    sha1.update(password);
    auto o1 = convertToCharArray(sha1.final());
    hz::SHA1 sha2;
    std::string p1(o1.begin(), o1.end());
    sha2.update(p1);
    sha3.update(seed);
    auto test2 = convertToCharArray(sha2.final());
    std::string p2(test2.begin(), test2.end());
    sha3.update(p2);
    std::string pass3 = sha3.final();
    auto p3 = convertToCharArray(pass3);
    for (unsigned i = 0; i < p3.size(); i++) {
        p3[i] = (char) (p3[i] ^ o1[i]);
    }
    return p3;
}


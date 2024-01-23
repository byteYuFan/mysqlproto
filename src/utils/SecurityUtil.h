//
// Created by wangyufan on 2024-01-15.
//

#ifndef PROTOOFMYSQL_SECURITYUTIL_H
#define PROTOOFMYSQL_SECURITYUTIL_H


#include <iostream>
#include <vector>
#include <cstring>
#include <openssl/sha.h>
std::vector<char >  scramble411(const std::string & pass, const std::string & seed);
#endif //PROTOOFMYSQL_SECURITYUTIL_H

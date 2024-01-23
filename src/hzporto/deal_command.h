//
// Created by wangyufan on 2024-01-22.
//

#ifndef PROTOOFMYSQL_DEAL_COMMAND_H
#define PROTOOFMYSQL_DEAL_COMMAND_H

#include "../HzMySQLClient.h"
#include <memory>

class HzDealCommand;



class HzDealCommand {
private:
    HzMySQLClient &hz_mysql_client_;
    QueryResult  queryResult;
public:
    explicit HzDealCommand(HzMySQLClient &hzMySqlClient) : hz_mysql_client_(hzMySqlClient) {}

    bool dealQuery(const std::string &query_sql);
};


#endif //PROTOOFMYSQL_DEAL_COMMAND_H

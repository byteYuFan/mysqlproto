//
// Created by wangyufan on 2024-01-22.
//

#include "deal_command.h"

bool HzDealCommand::dealQuery(const std::string &query_sql) {
    if (!hz_mysql_client_.query(query_sql)) {
        return false;
    }
    printf("\n");
    this->queryResult = this->hz_mysql_client_.query_result_;
    for (auto &cd: *queryResult.column_definition_) {
        cd->push_back('\0');
    }
    for (auto &cd: *queryResult.column_definition_) {
        printf("%s  ", cd->data());
    }
    printf("\n");
    for (int i = 0; i < queryResult.column_data_->size();) {
        printf("%s  ", queryResult.column_data_->at(i)->data());
        i++;
        if (i % queryResult.column_definition_->size() == 0)std::putchar('\n');
    }
    this->queryResult.column_definition_->clear();
    this->queryResult.column_data_->clear();
    this->hz_mysql_client_.query_result_.column_definition_->clear();
    this->hz_mysql_client_.query_result_.column_data_->clear();
    std::putchar('\n');
    return true;
}

//
// Created by 86158 on 2021/12/22.
//

#ifndef BOOKSTORE_2021_LOG_H
#define BOOKSTORE_2021_LOG_H

#include <fstream>
#include <iostream>
#include <vector>

#include "Unrolled_Linklist.h"
#include "Accounts.h"

enum Behavior {
    AddUser, Delete, Show, Buy, Select, Modify, Import
};

struct Log {
    User user;
    Behavior behavior;
    char description[150];
    bool if_earn;
    double amount;
};

class LogManagement {
private:
    std::fstream log_data_;
    std::string data_name_ = "log_data_storage";
    int log_num_;
    int head_preserved_ = sizeof(int) + 5;
public:
    LogManagement() {
        log_data_.open(data_name_);
        if (!log_data_) {
            log_data_.open(data_name_, std::ostream::out);
            log_data_.close();
            log_data_.open(data_name_);
            log_num_ = 0;
        } else {
            log_data_.seekg(0);
            log_data_.read(reinterpret_cast<char *>(&log_num_), sizeof(int));
        }
    }

    ~LogManagement() {
        log_data_.seekp(0);
        log_data_.write(reinterpret_cast<char *>(&log_num_), sizeof(int));
        log_data_.close();
    }

    void AddLog(Log &log_in) {
        log_data_.seekp(head_preserved_ + sizeof(Log) * log_num_);
        log_num_++;
        log_data_.write(reinterpret_cast<char *>(&log_in), sizeof(Log));
        return;
    }

    void ShowFinance(TokenScanner &input, AccountManagement &accounts) {
        if (accounts.GetCurrentPriority() < 7)
            throw "Invalid\n";
    }

    void ShowLog(TokenScanner &input) {// log command

    }
};

#endif //BOOKSTORE_2021_LOG_H

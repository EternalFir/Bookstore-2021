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
#include "InputCheck.h"

enum Behavior {
    AddUser, Delete, Show, Buy, Select, Modify, Import
};

struct Log {
    User user;
    Behavior behavior;
//    char description[150];
    bool if_earn;
    double amount;

    Log(){}

    Log(User user_in, Behavior behavior_in, bool if_earn_in, double amount_in) {
        user = user_in;
        behavior = behavior_in;
        if_earn = if_earn_in;
        amount = amount_in;
    }
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
            throw std::string("Invalid\n");
        std::string times_in_str = input.NextToken();
        double ans_add = 0.0;
        double ans_sub=0.0;
        Log temp;
        if (times_in_str.empty()) {
            log_data_.seekg(head_preserved_);
            for (int i = 0; i < log_num_; i++){
                log_data_.read(reinterpret_cast<char*>(&temp), sizeof(Log));
                if(temp.if_earn)
                    ans_add+=temp.amount;
                else
                    ans_sub+=temp.amount;
            }
        } else {
            CheckType6(times_in_str);
            int times_in = atoi(times_in_str.c_str());
            if (times_in > log_num_)
                throw std::string("Invalid\n");
            log_data_.seekg(head_preserved_+ sizeof(Log)*(log_num_-times_in));
            for(int i=0;i<times_in;i++)
                log_data_.read(reinterpret_cast<char*>(&temp), sizeof(Log));
            if(temp.if_earn)
                ans_add+=temp.amount;
            else
                ans_sub+=temp.amount;
        }
        std::cout.setf(std::ios::fixed);
        std::cout<<"+ ";
        std::cout<<std::setprecision(2)<<ans_add;
        std::cout<<" - ";
        std::cout<<std::setprecision(2)<<ans_sub<<std::endl;
    }

    void ShowLog(TokenScanner &input) {// log command

    }
};

#endif //BOOKSTORE_2021_LOG_H

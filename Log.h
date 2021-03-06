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
    AddUser, Delete, Show, Sell, Select, Modify, Import,
};

struct Log {
    UserID userID;
    Behavior behavior;
    bool if_earn;
    double amount;

    Log() {}

    Log(UserID userID_in, Behavior behavior_in, bool if_earn_in, double amount_in) {
        userID = userID_in;
        behavior = behavior_in;
        if_earn = if_earn_in;
        amount = amount_in;
    }
};

struct LogAll {
    UserID userID;
    Behavior behavior;
//    char description[150];
    double amount;
    char ISBN[21];
    char bookname[61];

    LogAll() {}

    LogAll(UserID userID_in, Behavior behavior_in, double amount_in, char ISBN_in[21], char bookname_in[61]) {
        userID = userID_in;
        behavior = behavior_in;
        amount = amount_in;
        strcpy(ISBN, ISBN_in);
        strcpy(bookname, bookname_in);
    }
};

class LogManagement {
private:
    std::fstream log_finance_data_;
    std::fstream log_all_data_;
    std::string data_name_ = "log_finance_data_storage";
    std::string data_all_name = "log_all_data_storage";
    int log_finance_num_;
    int log_all_num_;
    int head_preserved_ = sizeof(int) + 5;
public:
    LogManagement() {
        log_finance_data_.open(data_name_);
        if (!log_finance_data_) {
            log_finance_data_.open(data_name_, std::ostream::out);
            log_finance_data_.close();
            log_finance_data_.open(data_name_);
            log_finance_num_ = 0;
        } else {
            log_finance_data_.seekg(0);
            log_finance_data_.read(reinterpret_cast<char *>(&log_finance_num_), sizeof(int));
        }
        log_all_data_.open(data_all_name);
        if (!log_all_data_) {
            log_all_data_.open(data_all_name, std::ostream::out);
            log_all_data_.close();
            log_all_data_.open(data_all_name);
            log_finance_num_ = 0;
        } else {
            log_all_data_.seekg(0);
            log_all_data_.read(reinterpret_cast<char *>(&log_all_num_), sizeof(int));
        }
    }

    ~LogManagement() {
        log_finance_data_.seekp(0);
        log_finance_data_.write(reinterpret_cast<char *>(&log_finance_num_), sizeof(int));
        log_finance_data_.close();
        log_all_data_.seekp(0);
        log_all_data_.write(reinterpret_cast<char *>(&log_all_num_), sizeof(int));
        log_all_data_.close();
    }

    void AddLog(Log &log_in) {
        log_finance_data_.seekp(head_preserved_ + sizeof(Log) * log_finance_num_);
        log_finance_num_++;
        log_finance_data_.write(reinterpret_cast<char *>(&log_in), sizeof(Log));
        return;
    }

    void AddAllLog(LogAll &log_in) {
        log_all_data_.seekp(head_preserved_ + sizeof(LogAll) * log_all_num_);
        log_all_num_++;
        log_all_data_.write(reinterpret_cast<char *>(&log_in), sizeof(LogAll));
    }

    void ShowFinance(TokenScanner &input, AccountManagement &accounts) {
        if (accounts.GetCurrentPriority() < 7)
            throw std::string("Invalid\n");
        std::string times_in_str = input.NextToken();
        std::string trush_in = input.NextToken();
        if (!trush_in.empty())
            throw std::string("Invalid\n");
        double ans_add = 0.0;
        double ans_sub = 0.0;
        Log temp;
        if (times_in_str.empty()) {
            log_finance_data_.seekg(head_preserved_);
            for (int i = 0; i < log_finance_num_; i++) {
                log_finance_data_.read(reinterpret_cast<char *>(&temp), sizeof(Log));
                if (temp.if_earn)
                    ans_add += temp.amount;
                else
                    ans_sub += temp.amount;
            }
        } else {
//            CheckType6(times_in_str);
            int times_in = atoi(times_in_str.c_str());
            if (times_in > log_finance_num_)
                throw std::string("Invalid\n");
            log_finance_data_.seekg(head_preserved_ + sizeof(Log) * (log_finance_num_ - times_in));
            for (int i = 0; i < times_in; i++) {
                log_finance_data_.read(reinterpret_cast<char *>(&temp), sizeof(Log));
                if (temp.if_earn)
                    ans_add += temp.amount;
                else
                    ans_sub += temp.amount;
            }
        }
        std::cout.setf(std::ios::fixed);
        std::cout << "+ ";
        std::cout << std::setprecision(2) << ans_add;
        std::cout << " - ";
        std::cout << std::setprecision(2) << ans_sub << std::endl;
    }

    void Report(TokenScanner &input, AccountManagement &accounts) {
        std::string report_type_in = input.NextToken();
        std::string trush_in = input.NextToken();
        if (!trush_in.empty())
            throw std::string("Invalid\n");
        if (report_type_in == "myself") {
            if (accounts.GetCurrentPriority() < 3)
                throw std::string("Invalid\n");
            UserID my_ID = accounts.GetCurrentUser().GetID();
            std::cout << "\033[33maccount:\t" << my_ID << '\t';
            if (accounts.GetCurrentPriority() == 7)
                std::cout << "owner" << std::endl;
            else
                std::cout << "worker" << std::endl;
            std::cout << "\033[34m[operation]" << '\t' << "[ISBN]" << '\t' << "[bookname]" << '\t' << "[amount]\033[0m"
                      << std::endl;
            for (int i = 0; i < log_all_num_; i++) {
                LogAll temp;
                log_all_data_.seekg(head_preserved_ + i * sizeof(LogAll));
                log_all_data_.read(reinterpret_cast<char *>(&temp), sizeof(LogAll));
                if (temp.userID == my_ID) {
                    std::cout.setf(std::ios::fixed);
                    std::cout << BehaviorToStr(temp.behavior) << '\t' << temp.ISBN << '\t' << temp.bookname << '\t'
                              << std::setprecision(2) << temp.amount << std::endl;
                }
            }
            return;
        } else if (report_type_in == "finance") {
            if (accounts.GetCurrentPriority() < 7)
                throw std::string("Invalid\n");
            std::cout << "\033[31m-----Report Finance Situation-----\033[0m" << std::endl;
            std::cout << "\033[34m[Account]" << '\t' << "[operation]" << '\t' << "[ISBN]" << '\t' << "[bookname]"
                      << '\t' << "[amount]\033[0m" << std::endl;
            log_all_data_.seekg(head_preserved_);
            for (int i = 0; i < log_all_num_; i++) {
                LogAll temp;
                log_all_data_.read(reinterpret_cast<char *>(&temp), sizeof(LogAll));
                if (temp.behavior == 3) {
                    std::cout << temp.userID << '\t' << BehaviorToStr(temp.behavior) << '\t' << temp.ISBN << '\t'
                              << temp.bookname << '\t' << temp.amount << std::endl;
                } else if (temp.behavior == 6) {
                    std::cout << temp.userID << '\t' << BehaviorToStr(temp.behavior) << '\t' << temp.ISBN << '\t'
                              << temp.bookname << '\t' << temp.amount << std::endl;
                }
            }
            return;
        } else if (report_type_in == "employee") {
            if (accounts.GetCurrentPriority() < 7)
                throw std::string("Invalid\n");
            std::cout << "\033[31m-----Report Employee Situation-----\033[0m" << std::endl;
            std::cout << "\033[34m[operation]" << '\t' << "[ISBN]" << '\t' << "[bookname]"
                      << '\t' << "[amount]\033[0m" << std::endl;
            std::vector<UserID> employees;
            accounts.GetAllEmployee(employees);
            for (int i = 0; i < employees.size(); i++) {
                std::cout << "\033[33mreporting:\t" << employees[i] << std::endl;
                log_all_data_.seekg(head_preserved_);
                for (int j = 0; j < log_all_num_; j++) {
                    LogAll temp;
                    log_all_data_.read(reinterpret_cast<char *>(&temp), sizeof(LogAll));
                    if (temp.userID == employees[i]) {
                        std::cout.setf(std::ios::fixed);
                        std::cout << BehaviorToStr(temp.behavior) << '\t' << temp.ISBN << '\t' << temp.bookname << '\t'
                                  << std::setprecision(2) << temp.amount << std::endl;
                    }
                }
            }
            return;
        } else
            throw std::string("Invalid\n");
    }

    void ShowLog(TokenScanner &input, AccountManagement &accounts) {// log command
        if (accounts.GetCurrentPriority() < 7)
            throw std::string("Invalid\n");
        std::string trush_in = input.NextToken();
        if (!trush_in.empty())
            throw std::string("Invalid\n");
        std::cout << "\033[31m-----Show All Logs-----\033[0m" << std::endl;
        std::cout << "\033[34m[Account]" << '\t' << "[operation]" << '\t' << "[ISBN]" << '\t' << "[bookname]"
                  << '\t' << "[amount]\033[0m" << std::endl;
        log_all_data_.seekg(head_preserved_);
        for (int i = 0; i < log_all_num_; i++) {
            LogAll temp;
            log_all_data_.read(reinterpret_cast<char *>(&temp), sizeof(LogAll));
            std::cout << temp.userID << '\t' << BehaviorToStr(temp.behavior) << '\t' << temp.ISBN << '\t'
                      << temp.bookname << '\t' << temp.amount << std::endl;
        }
    }

    std::string BehaviorToStr(Behavior &behavior_in) {
        if (behavior_in == 0)
            return "AddUser";
        if (behavior_in == 1)
            return "Delete";
        if (behavior_in == 2)
            return "Show";
        if (behavior_in == 3)
            return "Sell";
        if (behavior_in == 4)
            return "Select";
        if (behavior_in == 5)
            return "modify";
        if (behavior_in == 6)
            return "Import";
        return "Undefined Operation.";
    }

};

#endif //BOOKSTORE_2021_LOG_H
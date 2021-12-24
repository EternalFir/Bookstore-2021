//
// Created by 86158 on 2021/12/22.
//

#ifndef BOOKSTORE_2021_ACCOUNTS_H
#define BOOKSTORE_2021_ACCOUNTS_H

#include <fstream>
#include <iostream>
#include <vector>

#include"Token_Scanner.h"
#include "Unrolled_Linklist.h"
//#include "Log.h"

class UserID {
private:
    char value_[31];
public:
    UserID() {};

    UserID(std::string userID) {
        strcpy(value_, userID.c_str());
    }

    void AddID(std::string in_ID) {
        strcpy(value_, in_ID.c_str());
    }

    [[nodiscard]] std::string GetUserID() const {
        std::string out;
        out = value_;
        return out;
    }

    bool operator==(const UserID &rhs) const {
        return strcmp(value_, rhs.value_) == 0;
    }

    bool operator!=(const UserID &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const UserID &rhs) const {
        return strcmp(value_, rhs.value_) < 0;
    }

    bool operator>(const UserID &rhs) const {
        return strcmp(value_, rhs.value_) > 0;
    }

    bool operator<=(const UserID &rhs) const {
        return strcmp(value_, rhs.value_) <= 0;
    }

    bool operator>=(const UserID &rhs) const {
        return strcmp(value_, rhs.value_) >= 0;
    }
};

class User {
private:
    int address_;// 该用户存储在第几个
    UserID userID_;
    char user_name_[31];
    char password_[31];
    int priority_;
public:
    User(){}

    User(int address_in, const std::string &ID_in, const std::string &password_in, const std::string &name_in,
         int priority_in) {
        address_ = address_in;
        userID_.AddID(ID_in);
        strcpy(user_name_, name_in.c_str());
        strcpy(password_, password_in.c_str());
        priority_ = priority_in;
    }

    void ChangePassword(const std::string &newpassword) {
        strcpy(password_, newpassword.c_str());
        return;
    }

    int GetAddress() const {
        return address_;
    }

    UserID GetID() const {
        return userID_;
    }

    [[nodiscard]] int GetPriority() const {
        return this->priority_;
    }

    bool check_password(const std::string &password_in) {
        if (strcmp(password_, password_in.c_str()) == 0)
            return true;
        return false;
    }
};

struct LogInAccount {
    User user;
    int selected_book_ID;
};

class AccountManagement {
private:
    std::vector<LogInAccount> log_in_;
    std::string data_name_ = "account_data_storage";
    std::fstream account_data_;
    UnrolledLinklist<UserID, int> ID_user_map_;
    int account_num_;// 记录已存储过的账号数量，1_based
    int head_preserved_ = sizeof(int) + 5;
public:
    AccountManagement() : ID_user_map_("account_index_storage") {
        account_data_.open(data_name_);
        if (!account_data_) {
            account_data_.open(data_name_, std::ostream::out);
            account_data_.close();
            account_data_.open(data_name_);
            account_num_ = 0;
            User root(account_num_, "root", "sjtu", "root", 7);
            account_num_++;
            account_data_.seekp(0);
            account_data_.write(reinterpret_cast<char *>(&account_num_), sizeof(int));
            account_data_.seekp(head_preserved_);
            account_data_.write(reinterpret_cast<char *>(&root), sizeof(User));
        } else {
            account_data_.seekg(0);
            account_data_.read(reinterpret_cast<char *>(&account_num_), sizeof(int));
        }
    }

    ~AccountManagement() {
        account_data_.seekp(0);
        account_data_.write(reinterpret_cast<char *>(&account_num_), sizeof(int));
        account_data_.close();
    }

    void SwitchUser(TokenScanner &input) {
        std::string ID_in, password_in;
        ID_in = input.NextToken();
        password_in = input.NextToken();
        User object;
        int find=-1;
        find = ID_user_map_.Get(ID_in);
        if (find == -1) {
            throw "Invalid\n";
        }
        account_data_.seekg(head_preserved_ + sizeof(User) * find);
        account_data_.read(reinterpret_cast<char *>(&object), sizeof(User));
        if (password_in.empty()) {
            if (object.GetPriority() < this->GetCurrentPriority()) {
                LogInAccount new_log;
                new_log.user = object;
                log_in_.push_back(new_log);
            } else
                throw "Invalid\n";
        } else {
            if (object.check_password(password_in)) {
                LogInAccount new_log;
                new_log.user = object;
                log_in_.push_back(new_log);
            } else
                throw "Invalid\n";
        }
    }

    void Logout() {
        if (this->GetCurrentPriority() <1)
            throw "Invalid\n";
        if (log_in_.empty())
            throw "Invalid\n";
        log_in_.erase(log_in_.end()--);
        return;
    }

    void Register(TokenScanner &input) {
        std::string ID_in, password_in, name_in;
        ID_in = input.NextToken();
        password_in = input.NextToken();
        name_in = input.NextToken();
        int find=-1;
        find = ID_user_map_.Get(ID_in);
        if (find != -1) {
            throw "Invalid\n";
        }
        User new_user(account_num_, ID_in, password_in, name_in, 1);
        account_num_++;
        account_data_.seekp(head_preserved_ + new_user.GetAddress() * sizeof(User));
        account_data_.write(reinterpret_cast<char *>(&new_user), sizeof(User));
        ID_user_map_.Insert(new_user.GetID(), new_user.GetAddress());
    }

    void ChangePassword(TokenScanner &input) {
        if (this->GetCurrentPriority() <1)
            throw "Invalid\n";
        std::string ID_in, old_password_in, new_password_in;
        ID_in = input.NextToken();
        old_password_in = input.NextToken();
        new_password_in = input.NextToken();
        int find=-1;
        find = ID_user_map_.Get(ID_in);
        if (find == -1) {
            throw "Invalid\n";
        }
        User object;
        account_data_.seekg(head_preserved_ + sizeof(User) * find);
        account_data_.read(reinterpret_cast<char *>(&object), sizeof(User));
        if (new_password_in.empty()) {// 省略旧密码情况
            new_password_in = old_password_in;
            if (this->GetCurrentPriority() == 7)
                object.ChangePassword(new_password_in);
            else
                throw "Invalid\n";
        } else {// 未省略旧密码
            if (object.check_password(old_password_in))
                object.ChangePassword(new_password_in);
            else
                throw "Invalid\n";
        }
        account_data_.seekp(head_preserved_ + sizeof(User) * object.GetAddress());
        account_data_.write(reinterpret_cast<char *>(&object), sizeof(User));
    }

    void UserAdd(TokenScanner &input) {
        if (this->GetCurrentPriority() < 3 )
            throw "Invalid\n";
        std::string ID_in, password_in, name_in;
        int priority_in;
        ID_in = input.NextToken();
        password_in = input.NextToken();
        priority_in = atoi(input.NextToken().c_str());
        name_in = input.NextToken();
        if (log_in_[log_in_.size() - 1].user.GetPriority() <= priority_in)
            throw "Invalid\n";
        int find=-1;
        find = ID_user_map_.Get(ID_in);
        if (find != -1) {
            throw "Invalid\n";
        }
        User new_user(account_num_, ID_in, password_in, name_in, priority_in);
        account_num_++;
        account_data_.seekp(head_preserved_ + new_user.GetAddress() * sizeof(User));
        account_data_.write(reinterpret_cast<char *>(&new_user), sizeof(User));
        ID_user_map_.Insert(new_user.GetID(), new_user.GetAddress());
    }

    void Delete(TokenScanner &input) {
        std::string ID_in;
        ID_in = input.NextToken();
        int find=-1;
        find = ID_user_map_.Get(ID_in);
        if (find == -1) {
            throw "Invalid\n";
        }
        for (int i = 0; i < log_in_.size(); i++) {
            if (log_in_[i].user.GetID() == ID_in)
                throw "Invalid\n";
        }
        ID_user_map_.Delete(ID_in, find);
    }

    void UserSelect(int book_id_in) {// 为当前用户选中对象
        log_in_[log_in_.size() - 1].selected_book_ID = book_id_in;
    }

    [[nodiscard]] int GetCurrentPriority() const {
        if(log_in_.empty())
            return -1;
        return log_in_[log_in_.size() - 1].user.GetPriority();
    }
};

#endif //BOOKSTORE_2021_ACCOUNTS_H

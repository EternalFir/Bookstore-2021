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
#include "InputCheck.h"
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

    friend std::ostream &operator<<(std::ostream &os, const UserID &id) {
        os << id.value_;
        return os;
    }
};

class User {
private:
    int address_;// 该用户存储在第几个
    UserID userID_;
    char user_name_[31];
    char password_[31];
    int priority_;
    char security_question_[61];
    char question_anwser_[61];
    bool if_set_question_ = false;
public:
    User() {}

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

    void SetQuestion(std::string &question_in, std::string &answer_in) {
        if_set_question_ = true;
        strcpy(security_question_, question_in.c_str());
        strcpy(question_anwser_, answer_in.c_str());
        return;
    }

    bool IfSetQusetion(){
        return if_set_question_;
    }

    std::string GetSecurityQuestion(){
        std::string ans=security_question_;
        return ans;
    }

    bool CheckAnswer(const std::string& answer_in){
        if (strcmp(question_anwser_, answer_in.c_str()) == 0)
            return true;
        return false;
    }
};

struct LogInAccount {
    User user;
    int selected_book_ID = -1;
};

class AccountManagement {
private:
    std::vector<LogInAccount> log_in_;
    std::string data_name_ = "account_data_storage";
    std::string data_ectype_name_="account_data_ectype_storage";
    std::fstream account_data_;
    std::fstream account_data_ectype_;
    UnrolledLinklist<UserID, int> ID_user_map_;
    int account_num_;// 记录已存储过的账号数量，1_based
    int head_preserved_ = sizeof(int) + 5;
    long last_save_time_;
public:
    AccountManagement() : ID_user_map_("account_index_storage","account_index_ectype_storage") {
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
            ID_user_map_.Insert(root.GetID(), root.GetAddress());
        } else {
            account_data_.seekg(0);
            account_data_.read(reinterpret_cast<char *>(&account_num_), sizeof(int));
        }
        account_data_ectype_.open(data_ectype_name_);
        if(!account_data_ectype_){
            account_data_ectype_.open(data_ectype_name_,std::ostream::out);
            account_data_ectype_.close();
        }
        last_save_time_=GetLastModifyTime();
        log_in_.clear();
        Save();
    }

    ~AccountManagement() {
        account_data_.seekp(0);
        account_data_.write(reinterpret_cast<char *>(&account_num_), sizeof(int));
        account_data_.close();
    }

    void SwitchUser(TokenScanner &input) {
        std::string ID_in, password_in, trush_in;
        ID_in = input.NextToken();
//        CheckType1(ID_in);
        password_in = input.NextToken();
//        CheckType1(password_in);
        trush_in = input.NextToken();
        if (!trush_in.empty())
            throw std::string("Invalid\n");
        User object;
        int find = -1;
        ID_user_map_.Get(ID_in, find);
        if (find == -1) {
            throw std::string("Invalid\n");
        }
        account_data_.seekg(head_preserved_ + sizeof(User) * find);
        account_data_.read(reinterpret_cast<char *>(&object), sizeof(User));
        if (password_in.empty()) {
            if (object.GetPriority() < this->GetCurrentPriority()) {
                LogInAccount new_log;
                new_log.user = object;
                log_in_.push_back(new_log);
            } else
                throw std::string("Invalid\n");
        } else {
            if (object.check_password(password_in)) {
                LogInAccount new_log;
                new_log.user = object;
                log_in_.push_back(new_log);
            } else
                throw std::string("Invalid\n");
        }
    }

    void Logout(TokenScanner &input) {
        if (this->GetCurrentPriority() < 1)
            throw std::string("Invalid\n");
        if (log_in_.empty())
            throw std::string("Invalid\n");
        std::string trush_in = input.NextToken();
        if (!trush_in.empty())
            throw std::string("Invalid\n");
        log_in_.pop_back();
        return;
    }

    void Register(TokenScanner &input) {
        std::string ID_in, password_in, name_in, trush_in;
        ID_in = input.NextToken();
//        CheckType1(ID_in);
        if (ID_in.empty())
            throw std::string("Invalid\n");
        password_in = input.NextToken();
//        CheckType1(password_in);
        if (password_in.empty())
            throw std::string("Invalid\n");
        name_in = input.NextToken();
//        CheckType2(name_in);
        trush_in = input.NextToken();
        if (!trush_in.empty())
            throw std::string("Invalid\n");
        int find = -1;
        ID_user_map_.Get(ID_in, find);
        if (find != -1) {
            throw std::string("Invalid\n");
        }
        User new_user(account_num_, ID_in, password_in, name_in, 1);
        account_num_++;
        account_data_.seekp(head_preserved_ + new_user.GetAddress() * sizeof(User));
        account_data_.write(reinterpret_cast<char *>(&new_user), sizeof(User));
        ID_user_map_.Insert(new_user.GetID(), new_user.GetAddress());
    }

    void ChangePassword(TokenScanner &input) {
        if (this->GetCurrentPriority() < 1)
            throw std::string("Invalid\n");
        std::string ID_in, old_password_in, new_password_in, trush_in;
        ID_in = input.NextToken();
//        CheckType1(ID_in);
        int find = -1;
        ID_user_map_.Get(ID_in, find);
        if (find == -1) {
            throw std::string("Invalid\n");
        }
        old_password_in = input.NextToken();
        new_password_in = input.NextToken();
        trush_in = input.NextToken();
        if (!trush_in.empty())
            throw std::string("Invalid\n");
        User object;
        account_data_.seekg(head_preserved_ + sizeof(User) * find);
        account_data_.read(reinterpret_cast<char *>(&object), sizeof(User));
        if (new_password_in.empty()) {// 省略旧密码情况
            new_password_in = old_password_in;
//            CheckType1(new_password_in);
            if (this->GetCurrentPriority() == 7)
                object.ChangePassword(new_password_in);
            else
                throw std::string("Invalid\n");
        } else {// 未省略旧密码
//            CheckType1(old_password_in);
//            CheckType1(new_password_in);
            if (object.check_password(old_password_in))
                object.ChangePassword(new_password_in);
            else
                throw std::string("Invalid\n");
        }
        account_data_.seekp(head_preserved_ + sizeof(User) * object.GetAddress());
        account_data_.write(reinterpret_cast<char *>(&object), sizeof(User));
    }

    void UserAdd(TokenScanner &input) {
        if (this->GetCurrentPriority() < 3)
            throw std::string("Invalid\n");
        std::string ID_in, password_in, name_in, trush_in;
        int priority_in;
        ID_in = input.NextToken();
        if (ID_in.empty())
            throw std::string("Invalid\n");
//        CheckType1(ID_in);
        password_in = input.NextToken();
//        CheckType1(password_in);
        std::string priority_in_str = input.NextToken();
//        CheckType3(priority_in_str);
        priority_in = atoi(priority_in_str.c_str());
        name_in = input.NextToken();
//        CheckType2(name_in);
        trush_in = input.NextToken();
        if (!trush_in.empty())
            throw std::string("Invalid\n");
        if (GetCurrentPriority() <= priority_in)
            throw std::string("Invalid\n");
        int find = -1;
        ID_user_map_.Get(ID_in, find);
        if (find != -1) {
            throw std::string("Invalid\n");
        }
        User new_user(account_num_, ID_in, password_in, name_in, priority_in);
        account_num_++;
        account_data_.seekp(head_preserved_ + new_user.GetAddress() * sizeof(User));
        account_data_.write(reinterpret_cast<char *>(&new_user), sizeof(User));
        ID_user_map_.Insert(new_user.GetID(), new_user.GetAddress());
    }

    void Delete(TokenScanner &input) {
        if (this->GetCurrentPriority() < 7)
            throw std::string("Invalid\n");
        std::string ID_in, trush_in;
        ID_in = input.NextToken();
//        CheckType1(ID_in);
        trush_in = input.NextToken();
        if (!trush_in.empty())
            throw std::string("Invalid\n");
        int find = -1;
        ID_user_map_.Get(ID_in, find);
        if (find == -1) {
            throw std::string("Invalid\n");
        }
        for (int i = 0; i < log_in_.size(); i++) {
            if (log_in_[i].user.GetID() == ID_in)
                throw std::string("Invalid\n");
        }
        ID_user_map_.Delete(ID_in, find);
    }

    void UserSelect(int book_id_in) {// 为当前用户选中对象
        log_in_[log_in_.size() - 1].selected_book_ID = book_id_in;
    }

    [[nodiscard]] int GetCurrentPriority() const {
        if (log_in_.empty())
            return -1;
        return log_in_[log_in_.size() - 1].user.GetPriority();
    }

    [[nodiscard]] int GetBookSelected() const {
        return log_in_[log_in_.size() - 1].selected_book_ID;
    }

    [[nodiscard]] User GetCurrentUser() const {
        return log_in_[log_in_.size() - 1].user;
    }

    void Debug() {
        std::cout << log_in_.size() << std::endl;
        for (int i = 0; i < log_in_.size(); ++i) {
            LogInAccount now = log_in_[i];
            std::cout << now.user.GetID() << '\t' << now.user.GetPriority() << std::endl;
        }
    }

    void GetAllEmployee(std::vector<UserID> &ans) {
        std::vector<int> address;
        ID_user_map_.TraverseAll(address);
        for (int i = 0; i < address.size(); i++) {
            User temp;
            account_data_.seekg(head_preserved_ + sizeof(User) * address[i]);
            account_data_.read(reinterpret_cast<char *>(&temp), sizeof(User));
            if (temp.GetPriority() == 3)
                ans.push_back(temp.GetID());
        }
    }

    void SetSecurityQuestion(TokenScanner &input) {
        std::string trush_in = input.NextToken();
        if (!trush_in.empty())
            throw std::string("Invalid\n");
        if (log_in_.empty())
            throw std::string("Invalid\n");
        User user_now = GetCurrentUser();
        std::cout << "\033[33mYou are setting your security question now." << std::endl;
        std::cout << "Enter " << "\033[31mgiveup" << "\033[33m to exit.\033[0m" << std::endl;
        std::cout << "Please enter your password:" << std::endl;
        std::string password_in, question_in, answer_in;
        while (1) {
            getline(std::cin, password_in);
            if (password_in == "giveup")
                return;
            if (!user_now.check_password(password_in)) {
                std::cout << "\033[31mWrong Password\033[0m" << std::endl;
                std::cout << "Please enter your password:" << std::endl;
            } else {
                std::cout << "Enter your security queston:" << std::endl;
                getline(std::cin, question_in);
                if (question_in == "giveup")
                    return;
                std::cout << "Enter your security question answer:" << std::endl;
                getline(std::cin, answer_in);
                if (answer_in == "giveup")
                    return;
                user_now.SetQuestion(question_in, answer_in);
                log_in_[log_in_.size() - 1].user = user_now;
                account_data_.seekp(head_preserved_ + sizeof(User) * user_now.GetAddress());
                account_data_.write(reinterpret_cast<char *>(&user_now), sizeof(User));
                std::cout << "Set successfully" << std::endl;
                return;
            }
        }
    }

    void ResetPassword(TokenScanner &input) {
        std::string password_in = input.NextToken();
        if (password_in != "password")
            throw std::string("Invalid\n");
        std::string ID_in_str = input.NextToken();
        if (ID_in_str.empty())
            throw std::string("Invalid\n");
        std::string trush_in = input.NextToken();
        if (!trush_in.empty())
            throw std::string("Invalid\n");
        UserID ID_in(ID_in_str);
        int find = -1;
        ID_user_map_.Get(ID_in, find);
        if (find == -1)
            throw std::string("Invalid\n");
        User changed_user;
        account_data_.seekg(head_preserved_+ sizeof(User)*find);
        account_data_.read(reinterpret_cast<char*>(&changed_user), sizeof(User));
        if(!changed_user.IfSetQusetion()){
            std::cout<<"Sorry, the account you select haven't set the security question yet."<<std::endl;
            return;
        }
        std::cout << "\033[33mYou are resetting your password now." << std::endl;
        std::cout << "Enter " << "\033[31mgiveup" << "\033[33m to exit.\033[0m" << std::endl;
        std::cout<<"Your security question is:"<<std::endl;
        std::cout<<"\033[32m"<<changed_user.GetSecurityQuestion()<<"\033[0m"<<std::endl;
        std::string answer_in,new_password_in,new_password_in_second;
        while (1) {
            std::cout<<"Please enter the answer of your security question."<<std::endl;
            getline(std::cin, answer_in);
            if (password_in == "giveup")
                return;
            if (!changed_user.CheckAnswer(answer_in)) {
                std::cout << "\033[31mWrong Answer\033[0m" << std::endl;
                std::cout << "Please enter your answer:" << std::endl;
            } else {
                std::cout<<"Answer passed."<<std::endl;
                std::cout << "Please enter your new password:" << std::endl;
                getline(std::cin, new_password_in);
                if (new_password_in == "giveup")
                    return;
                std::cout << "Please enter your new password again:" << std::endl;
                getline(std::cin, new_password_in_second);
                if (new_password_in_second == "giveup")
                    return;
                if(new_password_in==new_password_in_second){
                    changed_user.ChangePassword(new_password_in);
                    for(int j=0;j<log_in_.size();j++){
                        if(log_in_[j].user.GetID()==changed_user.GetID())
                            log_in_[j].user=changed_user;
                    }
                    account_data_.seekp(head_preserved_ + sizeof(User) * changed_user.GetAddress());
                    account_data_.write(reinterpret_cast<char *>(&changed_user), sizeof(User));
                    std::cout << "Set successfully" << std::endl;
                    return;
                }else{
                    std::cout << "\033[31mError: Different password. Please try again.\033[0m" << std::endl;
                }
            }
        }
    }

    long GetLastModifyTime() {
        account_data_.flush();
        struct stat buf;
        FILE *pFile = fopen(data_name_.c_str(), "r");
        int fd = fileno(pFile);
        fstat(fd, &buf);
        long time = buf.st_mtime;
        return time;
    }

    void Save() {
        ID_user_map_.Save();
        char temp;
        account_data_.seekg(0);
        account_data_ectype_.open(data_ectype_name_, std::ostream::out);
        account_data_ectype_.seekp(0);
        while (!account_data_.eof()) {
            account_data_.read(reinterpret_cast<char *>(&temp), sizeof(char));
            account_data_ectype_.write(reinterpret_cast<char *>(&temp), sizeof(char));
        }
        account_data_ectype_.close();


        account_data_.close();
        account_data_.open(data_name_);
        last_save_time_ = GetLastModifyTime();
    }

    void Load() {
        ID_user_map_.Load();
        char temp;
        account_data_.close();
        account_data_.open(data_name_, std::ostream::out);
        account_data_ectype_.open(data_ectype_name_);
        account_data_.seekp(0);
        account_data_ectype_.seekg(0);
        while (!account_data_ectype_.eof()) {
            account_data_ectype_.read(reinterpret_cast<char *>(&temp), sizeof(char));
            account_data_.write(reinterpret_cast<char *>(&temp), sizeof(char));
        }
        account_data_ectype_.close();
        account_data_.close();
        account_data_.open(data_name_);
        last_save_time_ = GetLastModifyTime();
    }

    void AutoSave() {
        ID_user_map_.AutoSave();
        long last_modify_time = GetLastModifyTime();
        if (last_modify_time == last_save_time_)
            return;
        else
            Save();
    }
};

#endif //BOOKSTORE_2021_ACCOUNTS_H
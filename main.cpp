#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <iomanip>
#include <cstdio>
#include <cctype>

#include "Unrolled_Linklist.h"
#include "Accounts.h"
#include "Log.h"
#include "Book.h"
#include "Token_Scanner.h"
#include "InputCheck.h"

//struct Char {
//    char value_[65];
//
//    bool operator==(const Char &rhs) const {
//        if (strcmp(value_, rhs.value_) == 0)
//            return true;
//        else
//            return false;
//    }
//
//    bool operator!=(const Char &rhs) const {
//        if (strcmp(value_, rhs.value_) != 0)
//            return true;
//        else
//            return false;
//    }
//
//    bool operator<(const Char &rhs) const {
//        return strcmp(value_, rhs.value_) < 0;
//    }
//
//    bool operator>(const Char &rhs) const {
//        return strcmp(value_, rhs.value_) > 0;
//    }
//
//    bool operator<=(const Char &rhs) const {
//        return strcmp(value_, rhs.value_) <= 0;
//    }
//
//    bool operator>=(const Char &rhs) const {
//        return strcmp(value_, rhs.value_) >= 0;
//    }
//};

//void Initialization();


int main() {
#ifndef ONLINE_JUDGE
    freopen("5.in","r",stdin);
    freopen("!output.txt","w",stdout);
#endif

    AccountManagement account_management;
    BookManagement book_management;
    std::string command_in;
    LogManagement log_management;
    while (getline(std::cin, command_in)) {
        try {

//            std::cout<<"# "<<command_in<<std::endl;

//            if(command_in=="select 978-0-9953009-4-1")
//                std::cout<<"debug"<<std::endl;



            CheckType0(command_in);
            TokenScanner command(command_in );
            std::string order;
            order = command.NextToken();
            if (order == "su") {
                account_management.SwitchUser(command);
            } else if (order == "logout") {
                account_management.Logout(command);
            } else if (order == "register") {
                account_management.Register(command);
            } else if (order == "passwd") {
                account_management.ChangePassword(command);
            } else if (order == "useradd") {
                account_management.UserAdd(command);
            } else if (order == "delete") {
                account_management.Delete(command);
            } else if (order == "show") {
                book_management.Show(command, account_management, log_management);
            } else if (order == "buy") {
                book_management.Buy(command, account_management, log_management);
            } else if (order == "select") {
                book_management.Select(command, account_management, log_management);
            } else if (order == "modify") {
                book_management.Modify(command, account_management, log_management);
            } else if (order == "import") {
                book_management.ImportBook(command, account_management, log_management);
            } else if (order == "quit")
                break;
            else if (order == "exit")
                break;
            else if (order.empty()) {
            }else if(order =="debug"){
                account_management.Debug();
            }
            else {
                throw std::string("Invalid\n");
            }
        }
        catch (std::string error_out) {
            std::cout << error_out;
        }
    }

    return 0;
}

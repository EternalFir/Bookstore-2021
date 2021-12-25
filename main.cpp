#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "Unrolled_Linklist.h"
#include <cstdio>
#include "Accounts.h"
#include "Log.h"
#include "Book.h"
#include "Token_Scanner.h"

struct Char {
    char value_[65];

    bool operator==(const Char &rhs) const {
        if (strcmp(value_, rhs.value_) == 0)
            return true;
        else
            return false;
    }

    bool operator!=(const Char &rhs) const {
        if (strcmp(value_, rhs.value_) != 0)
            return true;
        else
            return false;
    }

    bool operator<(const Char &rhs) const {
        return strcmp(value_, rhs.value_) < 0;
    }

    bool operator>(const Char &rhs) const {
        return strcmp(value_, rhs.value_) > 0;
    }

    bool operator<=(const Char &rhs) const {
        return strcmp(value_, rhs.value_) <= 0;
    }

    bool operator>=(const Char &rhs) const {
        return strcmp(value_, rhs.value_) >= 0;
    }
};

//void Initialization();


int main() {
//#ifndef ONLINE_JUDGE
//    freopen("5in","r",stdin);
//    freopen("!output.txt","w",stdout);
//#endif

    std::string command_in;
    AccountManagement account_management;
    BookManagement book_management;
    LogManagement log_management;
    while (getline(std::cin, command_in)) {
        try {
            TokenScanner command(command_in);
            std::string order;
            order = command.NextToken();
            if (order == "su") {
                account_management.SwitchUser(command);
            } else if (order == "logout") {
                account_management.Logout();
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
                book_management.Show(command, account_management, log_management);
            } else if (order == "modify") {
                book_management.Modify(command, account_management, log_management);
            } else if (order == "import") {
                book_management.ImportBook(command, account_management, log_management);
            }else{
                throw std::string("Invalid\n");
            }
        }
        catch (std::string error_out) {
            std::cout << error_out;
        }
    }

    return 0;
}

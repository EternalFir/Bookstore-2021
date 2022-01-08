#include <iostream>
#include <string>
#include <cstring>

#include "Unrolled_Linklist.h"
#include "Accounts.h"
#include "Log.h"
#include "Book.h"

int main() {
//#ifndef ONLINE_JUDGE
//    freopen("4.txt","r",stdin);
//    freopen("!output.txt","w",stdout);
//#endif

    std::cout << "\033[31m-----Welcome To The Bookstore-----" << std::endl;
    std::cout << "\033[35m Created By EternalFir\033[0m" << std::endl;
    AccountManagement account_management;
    BookManagement book_management;
    std::string command_in;
    LogManagement log_management;
    int times = 0;
    while (getline(std::cin, command_in)) {
        times++;
        if (times == 20) {
            times = 0;
            account_management.AutoSave();
            book_management.AutoSave();
            std::cout<<"Auto save completed!"<<std::endl;
        }
        try {
            TokenScanner command(command_in);
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
            } else if (order == "report") {
                log_management.Report(command, account_management);
            } else if (order == "log") {
                log_management.ShowLog(command, account_management);
            } else if (order == "set") {
                account_management.SetSecurityQuestion(command);
            } else if (order == "reset") {
                account_management.ResetPassword(command);
            } else if (order == "save") {
                times=0;
                account_management.AutoSave();
                book_management.AutoSave();
                std::cout<<"Save completed!"<<std::endl;
            } else if (order == "load") {
                times=0;
                account_management.Load();
                book_management.Load();
                std::cout<<"Load completed!"<<std::endl;
            } else if (order == "quit") {
                std::string trush_in = command.NextToken();
                if (!trush_in.empty())
                    throw std::string("Invalid\n");
                break;
            } else if (order == "exit") {
                std::string trush_in = command.NextToken();
                if (!trush_in.empty())
                    throw std::string("Invalid\n");
                break;
            } else if (order.empty()) {
            } else if (order == "debug") {
                account_management.Debug();
            } else {
                throw std::string("Invalid\n");
            }
        }
        catch (std::string error_out) {
            std::cout << error_out;
        }
    }
    return 0;
}
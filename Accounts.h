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
#include "Log.h"

class UserID{
private:
    char [31] value_;
public:
    UserID();
    UserID(std::string userID){
        strcpy(value_,userID);
    }
    std::string GetUserID() const {
        std::string out;
        out=value_;
        return out;
    }

    bool operator==(const UserID &rhs) const {
        return strcpy(value_,rhs.value_)==0;
    }

    bool operator!=(const UserID &rhs) const {
        return !(rhs == *this);
    }
};
#endif //BOOKSTORE_2021_ACCOUNTS_H

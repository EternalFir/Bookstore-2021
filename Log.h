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
struct Log{
//    User user;
    Behavior behavior;
    char description[150];
    bool if_earn;
    double amount;
};
#endif //BOOKSTORE_2021_LOG_H

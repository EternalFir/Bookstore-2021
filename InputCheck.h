//
// Created by 86158 on 2021/12/26.
//

#ifndef BOOKSTORE_2021_INPUTCHECK_H
#define BOOKSTORE_2021_INPUTCHECK_H

#include <iostream>
#include <string>
#include <cctype>


void CheckType0(const std::string &input) {
    if (input.length() > 1024)
        throw std::string("Invalid\n");
    for (int i = 0; i < input.length(); i++)
        if (input[i] == '\t' || (!isascii(input[i])))
            throw std::string("Invalid\n");
}

void CheckType1(const std::string &input) {
    if (input.length() > 30)
        throw std::string("Invalid\n");
    for (int i = 0; i < input.length(); i++) {
        char temp = input[i];
        if ((!isdigit(temp)) && (!isalpha(temp)) && (temp != '_'))
            throw std::string("Invalid\n");
    }
}

void CheckType2(const std::string &input) {
    if (input.length() > 30)
        throw std::string("Invalid\n");
    for (int i = 0; i < input.length(); i++)
        if (!(isascii(input[i]) && isprint(input[i])))
            throw std::string("Invalid\n");
}

void CheckType3(const std::string &input) {
    if (input.length() > 1)
        throw std::string("Invalid\n");
    if (!isdigit(input[0]))
        throw std::string("Invalid\n");
}

void CheckType4(const std::string &input) {
    if (input.length() > 20)
        throw std::string("Invalid\n");
    for (int i = 0; i < input.length(); i++)
        if (!(isascii(input[i]) && isprint(input[i])))
            throw std::string("Invalid\n");
}

void CheckType5(const std::string &input) {
    if (input.length() > 60)
        throw std::string("Invalid\n");
    for (int i = 0; i < input.length(); i++)
        if ((!isascii(input[i])) || (!isprint(input[i])) || input[i] == '"')
            throw std::string("Invalid\n");
}

void CheckType6(const std::string &input) {
    if (input.length() > 10)
        throw std::string("Invalid\n");
    for (int i = 0; i < input.length(); i++)
        if (!isdigit(input[i]))
            throw std::string("Invalid\n");
}

void CheckType7(const std::string &input) {
    if (input.length() > 13)
        throw std::string("Invalid\n");
    for (int i = 0; i < input.length(); i++)
        if ((!isdigit(input[i])) && input[i] != '.')
            throw std::string("Invalid\n");
}

#endif //BOOKSTORE_2021_INPUTCHECK_H

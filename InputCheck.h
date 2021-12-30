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
    if (input.length() > 30 || input.empty())
        throw std::string("Invalid\n");
    for (int i = 0; i < input.length(); i++)
        if (!(isascii(input[i]) && isprint(input[i])))
            throw std::string("Invalid\n");
}

void CheckType3(const std::string &input) {
    if (input.length() > 1 || input.empty())
        throw std::string("Invalid\n");
    if (!isdigit(input[0]))
        throw std::string("Invalid\n");
}

void CheckType4(const std::string &input) {
    if (input.length() > 20 || input.empty())
        throw std::string("Invalid\n");
    for (int i = 0; i < input.length(); i++)
        if (!(isascii(input[i]) && isprint(input[i])))
            throw std::string("Invalid\n");
}

void CheckType5(const std::string &input) {
    if (input.length() > 60 || input.empty())
        throw std::string("Invalid\n");
    for (int i = 0; i < input.length(); i++)
        if ((!isascii(input[i])) || (!isprint(input[i])) || input[i] == '"')
            throw std::string("Invalid\n");
}

void CheckType6(const std::string &input) {
    if (input.length() > 10 || input.empty())
        throw std::string("Invalid\n");
    for (int i = 0; i < input.length(); i++)
        if (!isdigit(input[i]))
            throw std::string("Invalid\n");
    if (atoi(input.c_str()) > 2147483647)
        throw std::string("Invalid\n");
}

void CheckType7(const std::string &input) {
    if (input.length() > 13 || input.empty())
        throw std::string("Invalid\n");
    for (int i = 0; i < input.length(); i++)
        if ((!isdigit(input[i])) && input[i] != '.')
            throw std::string("Invalid\n");
    int num = 0;
    for (int i = 0; i < input.length(); i++)
        if (input[i] == '.')
            num++;
    if (num > 1)
        throw std::string("Invalid\n");
}

void PairCheck(std::string input, char delimiter_in) {
    if (input[0] != delimiter_in)
        throw std::string("Invalid\n");
    for (int i = 1; i < input.length() - 1; i++) {
        if (input[i] == delimiter_in)
            throw std::string("Invalid\n");
    }
    if (input[input.length() - 1] != delimiter_in)
        throw std::string("Invalid\n");
}

void DelimiterCheck(std::string input, char delimiter_in) {
    int prev = -1;
    if (input[input.length() - 1] == delimiter_in)
        throw std::string("Invalid\n");
    for (int i = 0; i < input.length() - 1; i++) {
        if (input[i] == delimiter_in) {
            if ((i - prev) < 2)
                throw std::string("Invalid\n");
            prev = i;
        }
    }
}

#endif //BOOKSTORE_2021_INPUTCHECK_H

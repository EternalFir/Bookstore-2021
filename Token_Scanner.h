//
// Created by 86158 on 2021/12/22.
//
#ifndef BOOKSTORE_2021_TOKEN_SCANNER_H
#define BOOKSTORE_2021_TOKEN_SCANNER_H

#include <iostream>
#include <string>

class TokenScanner {
private:
    std::string buffer_ = "";
    int current_;
    char delimiter_ = ' ';
    int reader=0;// 记录当前输出的序号
    void cut(){// 将输入按照delimiter_切分

    }
public:
    TokenScanner();

    TokenScanner(char delimiter) {
        delimiter_ = delimiter;
    }

    TokenScanner(const std::string &in, char delimiter = ' ') {
        buffer_ = in;
        delimiter_ = delimiter;
    }

    ~TokenScanner();

    std::string NextToken() {
        std::string out;

    }

};

#endif //BOOKSTORE_2021_TOKEN_SCANNER_H

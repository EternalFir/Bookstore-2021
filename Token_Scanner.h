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
    int current_=0;
    char delimiter_ = ' ';
public:
    TokenScanner();

    TokenScanner(const TokenScanner& rhs){
        current_=rhs.current_;
        current_=rhs.current_;
        delimiter_=rhs.delimiter_;
        buffer_=rhs.buffer_;
    }
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
        int start = current_;
        int end = current_;
        while (buffer_[end] != delimiter_ && end != buffer_.length()) {
            end++;
        }
        out = buffer_.substr(start, end - start);
        current_ = end + 1;
        return out;
    }

    void Clear() {
        buffer_ = "";
        return;
    }

    friend std::istream& operator>>(std::istream input,std::string newbuffer){// 会重置读取状态
        buffer_=newbuffer;
        current_=0;
    }

    void SetDelimiter(char newdelimiter) {// 会重置读取状态
        current_=0;
        delimiter_ = newdelimiter;
        return;
    }
};

#endif //BOOKSTORE_2021_TOKEN_SCANNER_H

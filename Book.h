//
// Created by 86158 on 2021/12/22.
//

#ifndef BOOKSTORE_2021_BOOK_H
#define BOOKSTORE_2021_BOOK_H

#include <fstream>
#include <iostream>
#include <vector>

#include"Token_Scanner.h"
#include "Unrolled_Linklist_double_key.h"

struct ISBN {
    char value[21];

    ISBN(const std::string &in) {
        strcpy(value, in.c_str());
    }

    bool operator==(const ISBN &rhs) const {
        return strcmp(value, rhs.value) == 0;
    }

    bool operator!=(const ISBN &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const ISBN &rhs) const {
        return strcmp(value, rhs.value) < 0;
    }

    bool operator>(const ISBN &rhs) const {
        return strcmp(value, rhs.value) > 0;
    }

    bool operator<=(const ISBN &rhs) const {
        return strcmp(value, rhs.value) <= 0;
    }

    bool operator>=(const ISBN &rhs) const {
        return strcmp(value, rhs.value) >= 0;
    }
};

struct BookName {
    char value[61];

    BookName(const std::string &in) {
        strcpy(value, in.c_str());
    }

    bool operator==(const BookName &rhs) const {
        return strcmp(value, rhs.value) == 0;
    }

    bool operator!=(const BookName &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const BookName &rhs) const {
        return strcmp(value, rhs.value) < 0;
    }

    bool operator>(const BookName &rhs) const {
        return strcmp(value, rhs.value) > 0;
    }

    bool operator<=(const BookName &rhs) const {
        return strcmp(value, rhs.value) <= 0;
    }

    bool operator>=(const BookName &rhs) const {
        return strcmp(value, rhs.value) >= 0;
    }
};

struct Auther {
    char value[61];

    Auther(const std::string &in) {
        strcpy(value, in.c_str());
    }

    bool operator==(const Auther &rhs) const {
        return strcmp(value, rhs.value) == 0;
    }

    bool operator!=(const Auther &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const Auther &rhs) const {
        return strcmp(value, rhs.value) < 0;
    }

    bool operator>(const Auther &rhs) const {
        return strcmp(value, rhs.value) > 0;
    }

    bool operator<=(const Auther &rhs) const {
        return strcmp(value, rhs.value) <= 0;
    }

    bool operator>=(const Auther &rhs) const {
        return strcmp(value, rhs.value) >= 0;
    }
};

struct Keyword {
    char value[61];

    Keyword(const std::string &in) {
        strcpy(value, in.c_str());
    }

    bool operator==(const Keyword &rhs) const {
        return strcmp(value, rhs.value) == 0;
    }

    bool operator!=(const Keyword &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const Keyword &rhs) const {
        return strcmp(value, rhs.value) < 0;
    }

    bool operator>(const Keyword &rhs) const {
        return strcmp(value, rhs.value) > 0;
    }

    bool operator<=(const Keyword &rhs) const {
        return strcmp(value, rhs.value) <= 0;
    }

    bool operator>=(const Keyword &rhs) const {
        return strcmp(value, rhs.value) >= 0;
    }
};

class Book {
private:
    ISBN ISBN_;
    BookName book_name_;
    Auther auther_;
    Keyword keyword_;
    int quantity_ = 0;
    double price_ = 0.0;
    double total_cost_ = 0.0;
public:
    int book_ID_;

    Book();

    Book(int ID_in, const std::string &ISBN_in, const std::string bookname_in, const std::string &auther_in,
         const std::string &keyword_in, int quantity_in = 0, double price_in = 0.0, double total_cost_in = 0.0)
            : ISBN_(ISBN_in), book_name_(bookname_in), auther_(auther_in), keyword_(keyword_in) {
        book_ID_ = ID_in;
        quantity_=quantity_in;
        price_=price_in;
        total_cost_=total_cost_in;
    }

    Book(int ID_in, const std::string& ISBN_in): ISBN_(ISBN_in), book_name_(""), auther_(""), keyword_(""){
        book_ID_=ID_in;
    }


};

#endif //BOOKSTORE_2021_BOOK_H

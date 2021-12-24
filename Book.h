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

struct Author {
    char value[61];

    Author(const std::string &in) {
        strcpy(value, in.c_str());
    }

    bool operator==(const Author &rhs) const {
        return strcmp(value, rhs.value) == 0;
    }

    bool operator!=(const Author &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const Author &rhs) const {
        return strcmp(value, rhs.value) < 0;
    }

    bool operator>(const Author &rhs) const {
        return strcmp(value, rhs.value) > 0;
    }

    bool operator<=(const Author &rhs) const {
        return strcmp(value, rhs.value) <= 0;
    }

    bool operator>=(const Author &rhs) const {
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
    Author author_;
    Keyword keyword_;
    int quantity_ = 0;
    double price_ = 0.0;
    double total_cost_ = 0.0;
public:
    int book_ID_;

    Book();

    Book(int ID_in, const std::string &ISBN_in, const std::string bookname_in, const std::string &author_in,
         const std::string &keyword_in, int quantity_in = 0, double price_in = 0.0, double total_cost_in = 0.0)
            : ISBN_(ISBN_in), book_name_(bookname_in), author_(author_in), keyword_(keyword_in) {
        book_ID_ = ID_in;
        quantity_=quantity_in;
        price_=price_in;
        total_cost_=total_cost_in;
    }

    Book(int ID_in, const std::string& ISBN_in): ISBN_(ISBN_in), book_name_(""), author_(""), keyword_(""){
        book_ID_=ID_in;
    }

    friend std::ostream& operator<<(std::ostream& output,const Book& boook_out){

    }
};

class BookManagement{
private:
    std::fstream book_data_;
    std::string ="book_data_storage";
};
#endif //BOOKSTORE_2021_BOOK_H

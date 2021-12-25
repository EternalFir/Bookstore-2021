//
// Created by 86158 on 2021/12/22.
//

#ifndef BOOKSTORE_2021_BOOK_H
#define BOOKSTORE_2021_BOOK_H

#include <fstream>
#include <iostream>
#include <vector>

#include"Token_Scanner.h"
#include "Accounts.h"
#include "Log.h"
#include "Unrolled_Linklist_double_key.h"
#include "Unrolled_Linklist.h"

struct ISBN {
    char value[21];

    ISBN() {}

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

    friend std::ostream &operator<<(std::ostream &os, const ISBN &isbn) {
        os << isbn.value;
        return os;
    }
};

struct BookName {
    char value[61];

    BookName() {}

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

    friend std::ostream &operator<<(std::ostream &os, const BookName &name) {
        os << name.value;
        return os;
    }
};

struct Author {
    char value[61];

    Author() {}

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

    friend std::ostream &operator<<(std::ostream &os, const Author &author) {
        os << author.value;
        return os;
    }
};

struct Keyword {
    char value[61];

    Keyword() {};

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

    friend std::ostream &operator<<(std::ostream &os, const Keyword &keyword) {
        os << keyword.value;
        return os;
    }
};

class Book {
private:

public:
    ISBN ISBN_;
    BookName book_name_;
    Author author_;
    Keyword keyword_;
    double total_cost_ = 0.0;
    int book_ID_;
    int quantity_ = 0;
    double price_ = 0.0;
    bool if_just_ISBN_;// 是否仅有ISBN信息

    Book();

    Book(int ID_in, const std::string &ISBN_in, const std::string bookname_in, const std::string &author_in,
         const std::string &keyword_in, int quantity_in = 0, double price_in = 0.0, double total_cost_in = 0.0)
            : ISBN_(ISBN_in), book_name_(bookname_in), author_(author_in), keyword_(keyword_in) {
        book_ID_ = ID_in;
        quantity_ = quantity_in;
        price_ = price_in;
        total_cost_ = total_cost_in;
        if_just_ISBN_ = false;
    }

    Book(int ID_in, const std::string &ISBN_in) : ISBN_(ISBN_in), book_name_(""), author_(""), keyword_("") {
        book_ID_ = ID_in;
        if_just_ISBN_ = true;
    }

    friend std::ostream &operator<<(std::ostream &output, const Book &book_out) {
        output << book_out.ISBN_ << '\t' << book_out.book_name_ << '\t' << book_out.author_ << '\t' << book_out.keyword_
               << '\t' << book_out.price_ << '\t' << book_out.quantity_ << '\n';
        return output;
    }
};

class BookManagement {
private:
    std::fstream book_data_;
    std::string data_name_ = "book_data_storage";
    UnrolledLinklist<ISBN, int> ISBN_book_map_;
    UnrolledLinklist_double_key<BookName, ISBN, int> bookname_book_map_;
    UnrolledLinklist_double_key<Author, ISBN, int> author_book_map_;
    UnrolledLinklist_double_key<Keyword, ISBN, int> keyword_book_map_;
    int book_num_;// 记录已存储过的图书数量，1_based
    int head_preserved_ = sizeof(int) + 5;
public:
    BookManagement() : ISBN_book_map_("ISBN_index_storage"), bookname_book_map_("bookname_index_storage"),
                       author_book_map_("author_index_storage"), keyword_book_map_("keyword_index_storage") {
        book_data_.open(data_name_);
        if (!book_data_) {
            book_data_.open(data_name_, std::ostream::out);
            book_data_.close();
            book_data_.open(data_name_);
            book_num_ = 0;
        } else {
            book_data_.seekg(0);
            book_data_.read(reinterpret_cast<char *>(&book_num_), sizeof(int));
        }
    }

    ~BookManagement() {
        book_data_.seekp(0);
        book_data_.write(reinterpret_cast<char *>(&book_num_), sizeof(int));
        book_data_.close();
    }

    void Show(TokenScanner &input, AccountManagement &accounts, LogManagement &logs) {
        // 记得测一下TokenScanner 仅会出现一类show
        TokenScanner show_info(input.NextToken(), '=');
        std::string show_type = show_info.NextToken();
        std::vector<int> ans_address;
        std::vector<Book> ans;
        if (show_type.empty()) {// 无附加参数时
            ISBN_book_map_.TraverseAll(ans_address);
        } else if (show_type == "finance") {
            logs.ShowFinance(input);
        } else if (show_type == "-ISBN") {
            std::string ISBN_in = show_info.NextToken();
            if (ISBN_in.empty())
                throw "Invalid\n";
            ISBN_book_map_.Traverse(ans_address, ISBN_in);
        } else if (show_type == "-name") {
            TokenScanner name_show(show_info.NextToken(), '"');
            std::string name_in = name_show.NextToken();
            if (name_in.empty())
                throw "Invalid\n";
            bookname_book_map_.Traverse(ans_address, name_in);
        } else if (show_type == "-author") {
            TokenScanner author_show(show_info.NextToken(), '"');
            std::string author_in = author_show.NextToken();
            if (author_in.empty())
                throw "Invalid\n";
            bookname_book_map_.Traverse(ans_address, author_in);
        } else if (show_type == "-keyword") {
            TokenScanner keyword_show(show_info.NextToken(), '"');
            std::string keyword_in = keyword_show.NextToken();
            if (keyword_in.empty())
                throw "Invalid\n";
            for (int i = 0; i < keyword_in.length(); i++) {
                if (keyword_in[i] == '|')
                    throw "Invalid\n";
            }
            keyword_book_map_.Traverse(ans_address, keyword_in);
        } else
            throw "Invalid\n";
        Book temp;
        for (int i = 0; i < ans_address.size(); i++) {
            book_data_.seekg(head_preserved_ + sizeof(Book) * ans_address[i]);
            book_data_.read(reinterpret_cast<char *>(&temp), sizeof(Book));
            ans[i] = temp;
        }
        if (ans.empty())
            std::cout << '\n';
        else {
            for (int i = 0; i < ans.size(); i++) {
                std::cout << ans[i].ISBN_ << '\t';
                std::cout << ans[i].book_name_ << '\t';
                std::cout << ans[i].author_ << '\t';
                std::cout << ans[i].keyword_ << '\t';
                std::cout << ans[i].price_ << '\t';
                std::cout << ans[i].quantity_ << '\n';
            }
        }
    }

    double Buy(TokenScanner &input, AccountManagement &accounts, LogManagement &logs) {
        if (accounts.GetCurrentPriority() < 1)
            throw "Invalid\n";
        std::string ISBN_in = input.NextToken();
        int quantity_in = atoi(input.NextToken().c_str());
        ISBN book_find_ISBN(ISBN_in);
        int book_place = ISBN_book_map_.Get(book_find_ISBN);
        if (book_place == -1)
            throw "Invalid\n";
        Book book_find;
        book_data_.seekg(head_preserved_ + book_place * sizeof(Book));
        book_data_.read(reinterpret_cast<char *>(&book_find), sizeof(Book));
        if (book_find.quantity_ < quantity_in)
            throw "Invalid\n";
        book_find.quantity_ -= quantity_in;
        double cost = double(book_find.price_ * quantity_in);
        return cost;
    }

    void Select(TokenScanner &input, AccountManagement &accounts, LogManagement &logs) {
        if (accounts.GetCurrentPriority() < 3)
            throw "Invalid\n";
        std::string ISBN_in;
        ISBN_in = input.NextToken();
        ISBN book_find_ISBN(ISBN_in);
        if (ISBN_book_map_.Get(book_find_ISBN) == -1) {// 找不到就添加书
            Book new_book(book_num_, ISBN_in);
            book_num_++;
            book_data_.seekp(head_preserved_ + new_book.book_ID_ * sizeof(Book));
            book_data_.write(reinterpret_cast<char *>(&new_book), sizeof(Book));
            ISBN_book_map_.Insert(ISBN_in, new_book.book_ID_);// 此时还无法插入到其余的三张表中
        } else {
            accounts.UserSelect(ISBN_book_map_.Get(book_find_ISBN));
        }
        return;
    }

    void Modify(TokenScanner &input, AccountManagement &accounts, LogManagement &logs) {
        if (accounts.GetCurrentPriority() < 3)
            throw "Invalid\n";
        if (accounts.GetBookSelected() == -1)
            throw "Invalid\n";
        Book modified_book;
        book_data_.seekg(head_preserved_ + sizeof(Book) * accounts.GetBookSelected());
        book_data_.read(reinterpret_cast<char *>(&modified_book), sizeof(Book));
        bool if_modify_type[5] = {false};
        TokenScanner modify_command_single("=");
        TokenScanner bookname_modify('"');
        TokenScanner author_modify('"');
        TokenScanner keyword_modify('"');
        std::string single_command = input.NextToken();
        if (single_command.empty())
            throw "Invalid\n";
        while (!single_command.empty()) {
            modify_command_single.SetBuffer(single_command);
            std::string command_type = modify_command_single.NextToken();
            if (command_type == "-ISBN") {
                if (if_modify_type[0])
                    throw "Invalid\n";
                if_modify_type[0] = true;
                std::string ISBN_in = modify_command_single.NextToken();
                ISBN temp(ISBN_in);
                ISBN_book_map_.Delete(modified_book.ISBN_,modified_book.book_ID_);
                modified_book.ISBN_ = temp;
                ISBN_book_map_.Insert(modified_book.ISBN_,modified_book.book_ID_);
            } else if (command_type == "-name") {
                if (if_modify_type[1])
                    throw "Invalid\n";
                if_modify_type[1] = true;

            }

            single_command = input.NextToken();
        }
//       TODO:  if_just_isbn!!!!


        book_data_.seekp(head_preserved_ + sizeof(Book) * modified_book.book_ID_);
        book_data_.write(reinterpret_cast<char *>(&modified_book), sizeof(Book));
    }

    void ImportBook(TokenScanner &input, AccountManagement &accounts, LogManagement &logs) {
        if (accounts.GetCurrentPriority() < 3)
            throw "Invalid\n";
        if (accounts.GetBookSelected() == -1)
            throw "Invalid\n";
        int quantity_in = atoi(input.NextToken().c_str());
        int total_cost_in = atoi(input.NextToken().c_str());
        Book changed_book;
        book_data_.seekg(head_preserved_ + accounts.GetBookSelected() * sizeof(Book));
        book_data_.read(reinterpret_cast<char *>(&changed_book), sizeof(Book));
        changed_book.quantity_ += quantity_in;
        changed_book.total_cost_ += total_cost_in;
        book_data_.seekp(head_preserved_ + changed_book.book_ID_ * sizeof(Book));
        book_data_.write(reinterpret_cast<char *>(&changed_book), sizeof(Book));
    }
};

#endif //BOOKSTORE_2021_BOOK_H

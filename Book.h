//
// Created by 86158 on 2021/12/22.
//

#ifndef BOOKSTORE_2021_BOOK_H
#define BOOKSTORE_2021_BOOK_H

#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>

#include"Token_Scanner.h"
#include "Accounts.h"
#include "Log.h"
#include "Unrolled_Linklist_double_key.h"
#include "Unrolled_Linklist.h"
#include "InputCheck.h"

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
//    bool if_just_ISBN_;// 是否仅有ISBN信息 TODO: 并不需要，Delete找不到便无变化

    Book() {}

    Book(int ID_in, const std::string &ISBN_in, const std::string bookname_in, const std::string &author_in,
         const std::string &keyword_in, int quantity_in = 0, double price_in = 0.0, double total_cost_in = 0.0)
            : ISBN_(ISBN_in), book_name_(bookname_in), author_(author_in), keyword_(keyword_in) {
        book_ID_ = ID_in;
        quantity_ = quantity_in;
        price_ = price_in;
        total_cost_ = total_cost_in;
//        if_just_ISBN_ = false;
    }

    Book(int ID_in, const std::string &ISBN_in) : ISBN_(ISBN_in), book_name_(""), author_(""), keyword_("") {
        book_ID_ = ID_in;
//        if_just_ISBN_ = true;
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
        if (accounts.GetCurrentPriority() < 1)
            throw std::string("Invalid\n");
        std::string temp_in=input.NextToken();
        if(temp_in=="finance")
            logs.ShowFinance(input, accounts);
        TokenScanner show_info(temp_in, '=');
        std::string show_type = show_info.NextToken();
        std::vector<int> ans_address;
        std::vector<Book> ans;
        if (show_type.empty()) {// 无附加参数时
            ISBN_book_map_.TraverseAll(ans_address);
        }else if (show_type == "-ISBN") {
            std::string ISBN_in = show_info.GetRest();
            if (ISBN_in.empty())
                throw std::string("Invalid\n");
            CheckType4(ISBN_in);
            ISBN_book_map_.Traverse(ans_address, ISBN_in);
        } else if (show_type == "-name") {
            std::string temp=show_info.NextToken();
            PairCheck(temp,'"');
            TokenScanner name_show(temp, '"');
            std::string name_in = name_show.NextToken();
            if (name_in.empty())
                throw std::string("Invalid\n");
            CheckType5(name_in);
            bookname_book_map_.Traverse(ans_address, name_in);
        } else if (show_type == "-author") {
            std::string temp=show_info.NextToken();
            PairCheck(temp,'"');
            TokenScanner author_show(temp, '"');
            std::string author_in = author_show.NextToken();
            if (author_in.empty())
                throw std::string("Invalid\n");
            CheckType5(author_in);
            author_book_map_.Traverse(ans_address, author_in);
        } else if (show_type == "-keyword") {
            std::string temp=show_info.NextToken();
            PairCheck(temp,'"');
            TokenScanner keyword_show(temp, '"');
            std::string keyword_in = keyword_show.NextToken();
            if (keyword_in.empty())
                throw std::string("Invalid\n");
            CheckType5(keyword_in);
            for (int i = 0; i < keyword_in.length(); i++) {
                if (keyword_in[i] == '|')
                    throw std::string("Invalid\n");
            }
            keyword_book_map_.Traverse(ans_address, keyword_in);
        } else
            throw std::string("Invalid\n");
        Book temp;
        for (int i = 0; i < ans_address.size(); i++) {
            book_data_.seekg(head_preserved_ + sizeof(Book) * ans_address[i]);
            book_data_.read(reinterpret_cast<char *>(&temp), sizeof(Book));
            ans.push_back(temp);
        }
        if (ans.empty())
            std::cout << '\n';
        else {
            for (int i = 0; i < ans.size(); i++) {
                std::cout.setf(std::ios::fixed);
                std::cout << ans[i].ISBN_ << '\t';
                std::cout << ans[i].book_name_ << '\t';
                std::cout << ans[i].author_ << '\t';
                std::cout << ans[i].keyword_ << '\t';
                std::cout << std::setprecision(2) << ans[i].price_ << '\t';
                std::cout << ans[i].quantity_ << '\n';

            }
        }
    }

    void Buy(TokenScanner &input, AccountManagement &accounts, LogManagement &logs) {
        if (accounts.GetCurrentPriority() < 1)
            throw std::string("Invalid\n");
        std::string ISBN_in = input.NextToken();
        CheckType4(ISBN_in);
        std::string quantity_in_str = input.NextToken();
        CheckType6(quantity_in_str);
        std::string trush_in=input.NextToken();
        if(!trush_in.empty())
            throw std::string("Invalid\n");
        int quantity_in = atoi(quantity_in_str.c_str());
        ISBN book_find_ISBN(ISBN_in);
        int book_place=-1;
        ISBN_book_map_.Get(book_find_ISBN, book_place);
        if (book_place == -1)
            throw std::string("Invalid\n");
        Book book_find;
        book_data_.seekg(head_preserved_ + book_place * sizeof(Book));
        book_data_.read(reinterpret_cast<char *>(&book_find), sizeof(Book));
        if (book_find.quantity_ < quantity_in)
            throw std::string("Invalid\n");
        book_find.quantity_ -= quantity_in;
        book_data_.seekp(head_preserved_ + book_place * sizeof(Book));
        book_data_.write(reinterpret_cast<char *>(&book_find), sizeof(Book));
        double cost = double(book_find.price_ * quantity_in);
        Log new_log(accounts.GetCurrentUser(), Behavior(3), true, cost);
        logs.AddLog(new_log);
        std::cout.setf(std::ios::fixed);
        std::cout << std::setprecision(2) << cost << std::endl;
    }

    void Select(TokenScanner &input, AccountManagement &accounts, LogManagement &logs) {
        if (accounts.GetCurrentPriority() < 3)
            throw std::string("Invalid\n");
        std::string ISBN_in = input.NextToken();
        CheckType4(ISBN_in);
        std::string trush_in=input.NextToken();
        if(!trush_in.empty())
            throw std::string("Invalid\n");
        ISBN book_find_ISBN(ISBN_in);
        int find = -1;
        ISBN_book_map_.Get(book_find_ISBN, find);
        if (find == -1) {// 找不到就添加书
            Book new_book(book_num_, ISBN_in);
            book_num_++;
            book_data_.seekp(head_preserved_ + new_book.book_ID_ * sizeof(Book));
            book_data_.write(reinterpret_cast<char *>(&new_book), sizeof(Book));
            ISBN_book_map_.Insert(ISBN_in, new_book.book_ID_);// 此时还无法插入到其余的三张表中
            accounts.UserSelect(new_book.book_ID_);
        } else {
            accounts.UserSelect(find);
        }
        return;
    }

    void Modify(TokenScanner &input, AccountManagement &accounts, LogManagement &logs) {
        if (accounts.GetCurrentPriority() < 3)
            throw std::string("Invalid\n");
        if (accounts.GetBookSelected() == -1)
            throw std::string("Invalid\n");
        Book modified_book;
        book_data_.seekg(head_preserved_ + sizeof(Book) * accounts.GetBookSelected());
        book_data_.read(reinterpret_cast<char *>(&modified_book), sizeof(Book));
        bool if_modify_type[5] = {false};
        TokenScanner modify_command_single('=');
        std::string ISBN_in;
        TokenScanner bookname_modify('"');
        std::string bookname_in;
        TokenScanner author_modify('"');
        std::string author_in;
        TokenScanner keyword_modify('"');
        std::string keywords_in;
        std::vector<std::string> keyword_in;
        std::vector<std::string> keyword_raw;
        double price_in;
        TokenScanner keywords_raw(modified_book.keyword_.value,'|');
        std::string keyword = keywords_raw.NextToken();
        while (!keyword.empty()) {
            keyword_raw.push_back(keyword);
            keyword = keywords_raw.NextToken();
        }
        std::string single_command = input.NextToken();
        if (single_command.empty())
            throw std::string("Invalid\n");
        while (!single_command.empty()) {
            modify_command_single.SetBuffer(single_command);
            std::string command_type = modify_command_single.NextToken();
            if (command_type == "-ISBN") {
                if (if_modify_type[0])
                    throw std::string("Invalid\n");
                if_modify_type[0] = true;
                ISBN_in = modify_command_single.NextToken();
                CheckType4(ISBN_in);
                ISBN temp(ISBN_in);
                int find = -1;
                ISBN_book_map_.Get(temp, find);
                if (find != -1)
                    throw std::string("Invalid\n");
                if (temp == modified_book.ISBN_)
                    throw std::string("Invalid\n");
//                ISBN_book_map_.Delete(modified_book.ISBN_, modified_book.book_ID_);
//                modified_book.ISBN_ = temp;
//                ISBN_book_map_.Insert(modified_book.ISBN_, modified_book.book_ID_);
            } else if (command_type == "-name") {
                if (if_modify_type[1])
                    throw std::string("Invalid\n");
                if_modify_type[1] = true;
                std::string temp=modify_command_single.NextToken();
                PairCheck(temp,'"');
                bookname_modify.SetBuffer(temp);
                bookname_in = bookname_modify.NextToken();
                CheckType5(bookname_in);
//                BookName temp(bookname_in);
//                bookname_book_map_.Delete(temp, modified_book.ISBN_, modified_book.book_ID_);
//                modified_book.book_name_ = temp;
//                bookname_book_map_.Insert(temp, modified_book.ISBN_, modified_book.book_ID_);
            } else if (command_type == "-author") {
                if (if_modify_type[2])
                    throw std::string("Invalid\n");
                if_modify_type[2] = true;
                std::string temp=modify_command_single.NextToken();
                PairCheck(temp,'"');
                author_modify.SetBuffer(temp);
                author_in = author_modify.NextToken();
                CheckType5(author_in);
//                Author temp(author_in);
//                author_book_map_.Delete(temp, modified_book.ISBN_, modified_book.book_ID_);
//                modified_book.author_ = temp;
//                author_book_map_.Insert(temp, modified_book.ISBN_, modified_book.book_ID_);
            } else if (command_type == "-keyword") {
                if (if_modify_type[3])
                    throw std::string("Invalid\n");
                if_modify_type[3] = true;
                std::string temp=modify_command_single.NextToken();
                PairCheck(temp,'"');
                keyword_modify.SetBuffer(temp);
                keywords_in = keyword_modify.NextToken();
                CheckType5(keywords_in);
//                DelimiterCheck(keywords_in,'|');
                TokenScanner keywords(keywords_in, '|');
                keyword = keywords.NextToken();
                while (!keyword.empty()) {
                    for (int j = 0; j < keyword_in.size(); j++) {
                        if (keyword == keyword_in[j])
                            throw std::string("Invalid\n");
                    }
                    keyword_in.push_back(keyword);
                    keyword = keywords.NextToken();
                }
            } else if (command_type == "-price") {
                if (if_modify_type[4])
                    throw std::string("Invalid\n");
                if_modify_type[4] = true;
                std::string price_in_str = modify_command_single.NextToken();
                CheckType7(price_in_str);
                price_in = atof(price_in_str.c_str());
//                modified_book.price_ = price_in;
            } else {
                throw std::string("Invalid\n");
            }
            single_command = input.NextToken();
        }
// 全部分析完成后再进行实际更改
// 先全部清除
        ISBN_book_map_.Delete(modified_book.ISBN_, modified_book.book_ID_);
        bookname_book_map_.Delete(modified_book.book_name_, modified_book.ISBN_, modified_book.book_ID_);
        author_book_map_.Delete(modified_book.author_, modified_book.ISBN_, modified_book.book_ID_);
        for (int i = 0; i < keyword_raw.size(); i++) {
            Keyword temp(keyword_raw[i]);
            keyword_book_map_.Delete(temp, modified_book.ISBN_, modified_book.book_ID_);
        }
// 再作更改
        if (if_modify_type[0]) {
            ISBN temp(ISBN_in);
            modified_book.ISBN_ = temp;
        }
        if (if_modify_type[1]) {
            BookName temp(bookname_in);
            modified_book.book_name_ = temp;
        }
        if (if_modify_type[2]) {
            Author temp(author_in);
            modified_book.author_ = temp;
        }
        if (if_modify_type[3]) {
            Keyword temp(keywords_in);
            modified_book.keyword_ = temp;
        }
        if (if_modify_type[4]) {
            modified_book.price_ = price_in;
        }
// 再全部加入
        ISBN_book_map_.Insert(modified_book.ISBN_, modified_book.book_ID_);
        bookname_book_map_.Insert(modified_book.book_name_, modified_book.ISBN_, modified_book.book_ID_);
        author_book_map_.Insert(modified_book.author_, modified_book.ISBN_, modified_book.book_ID_);
        if (if_modify_type[3]) {
            for (int i = 0; i < keyword_in.size(); i++) {
                Keyword temp(keyword_in[i]);
                keyword_book_map_.Insert(temp, modified_book.ISBN_, modified_book.book_ID_);
            }
        } else {
            for (int i = 0; i < keyword_raw.size(); i++) {
                Keyword temp(keyword_raw[i]);
                keyword_book_map_.Insert(temp, modified_book.ISBN_, modified_book.book_ID_);
            }
        }
        book_data_.seekp(head_preserved_ + sizeof(Book) * modified_book.book_ID_);
        book_data_.write(reinterpret_cast<char *>(&modified_book), sizeof(Book));
    }

    void ImportBook(TokenScanner &input, AccountManagement &accounts, LogManagement &logs) {
        if (accounts.GetCurrentPriority() < 3)
            throw std::string("Invalid\n");
        if (accounts.GetBookSelected() == -1)
            throw std::string("Invalid\n");
        std::string quantity_in_str = input.NextToken();
        CheckType6(quantity_in_str);
        int quantity_in = atoi(quantity_in_str.c_str());
        std::string total_cost_in_str = input.NextToken();
        CheckType7(total_cost_in_str);
        double total_cost_in = atof(total_cost_in_str.c_str());
        std::string trush_in=input.NextToken();
        if(!trush_in.empty())
            throw std::string("Invalid\n");
        Book changed_book;
        book_data_.seekg(head_preserved_ + accounts.GetBookSelected() * sizeof(Book));
        book_data_.read(reinterpret_cast<char *>(&changed_book), sizeof(Book));
        changed_book.quantity_ += quantity_in;
        changed_book.total_cost_ += total_cost_in;
        book_data_.seekp(head_preserved_ + changed_book.book_ID_ * sizeof(Book));
        book_data_.write(reinterpret_cast<char *>(&changed_book), sizeof(Book));
        Log new_log(accounts.GetCurrentUser(), Behavior(6), false, total_cost_in);
        logs.AddLog(new_log);
    }
};

#endif //BOOKSTORE_2021_BOOK_H

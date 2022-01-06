//
// Created by 86158 on 2021/12/10.
//
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <typeinfo>
#include <vector>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#ifndef BOOKSTORE_2021_UNROLLED_LINKLIST_H
#define BOOKSTORE_2021_UNROLLED_LINKLIST_H
const int kMaxNumPerBlock = 360;

//单键版本
template<typename key_type, typename value_type>
class UnrolledLinklist {
public:
    struct KeyValuePair {// 封装的最小存储单元
        key_type key;
        value_type value;

        KeyValuePair() {
            key = key_type();
            value = value_type();
        }

        bool operator<(const KeyValuePair &rhs) const {
            return key < rhs.key;
        }

        bool operator>(const KeyValuePair &rhs) const {
            return rhs < *this;
        }

        bool operator<=(const KeyValuePair &rhs) const {
            return !(rhs < *this);
        }

        bool operator>=(const KeyValuePair &rhs) const {
            return !(*this < rhs);
        }

        bool operator==(const KeyValuePair &rhs) const {
            return key == rhs.key &&
                   value == rhs.value;
        }

        bool operator!=(const KeyValuePair &rhs) const {
            return !(rhs == *this);
        }
    };

private:


    class Block {
    private:

    public:
//        bool if_occupied;// 当前块是否已经使用

        // 目前尚未实现并块操作
        // 可以用另一个单链表实现对释放的空间的管理

        KeyValuePair value_[kMaxNumPerBlock + 10];
//        KeyValuePair first_one;
//        KeyValuePair last_one;

        Block() {
            next_num_ = -1,
            prev_num_ = -1,
            elements_num_ = 0;
        }

        Block(int num) {
            next_num_ = -1;
            prev_num_ = -1;
            elements_num_ = 0;
            my_num_ = num;
        }

        int elements_num_;// 1_based
        int next_num_;
        int prev_num_;
        int my_num_ = 0;// 为第几个块，0_based
        KeyValuePair next_min_;// 后一个块中的最小值
    };


    std::fstream index_;
    std::string index_name_;
    std::fstream index_ectype_;
    std::string index_ectype_name_;

    const unsigned int head_preserved = sizeof(int) * 3 + 2 * sizeof(Block) + 10;// 这个的存储是否会出问题？
    int head_num = -1;// 0_based
    int tail_num = -2;
    int block_num;// 已经使用过的块数目

    long last_save_time_=0;

public:

    UnrolledLinklist(std::string in, std::string in_2) {
        index_name_ = in;
        index_.open(index_name_);
        if (!index_) {
            index_.open(index_name_, std::ostream::out);
            head_num = -1;
            tail_num = -2;
            Block head(head_num);
            head.next_num_ = 0;
            Block tail(tail_num);
            tail.prev_num_ = 0;
            Block f(0);
            f.prev_num_ = -1;
            f.next_num_ = -2;
            index_.seekp(head_preserved + head_num * sizeof(Block));
            index_.write(reinterpret_cast<char *>(&head), sizeof(Block));
            index_.seekp(head_preserved + tail_num * sizeof(Block));
            index_.write(reinterpret_cast<char *>(&tail), sizeof(Block));
            index_.seekp(head_preserved);
            index_.write(reinterpret_cast<char *>(&f), sizeof(Block));
            block_num = 1;
            index_.close();
            index_.open(index_name_);
        } else {
            index_.seekg(0);
            index_.read(reinterpret_cast<char *>(&head_num), sizeof(int));
            index_.read(reinterpret_cast<char *>(&tail_num), sizeof(int));
            index_.read(reinterpret_cast<char *>(&block_num), sizeof(int));
        }
        index_ectype_name_ = in_2;
        index_ectype_.open(index_ectype_name_);
        if (!index_ectype_) {
            index_ectype_.open(index_ectype_name_, std::ostream::out);
            index_ectype_.close();
        }
        last_save_time_ = GetLastModifyTime();
        Save();
//        index_.close();
    }

    ~UnrolledLinklist() {
//        index_.open(index_name_);
        index_.seekp(0);
        index_.write(reinterpret_cast<char *>(&head_num), sizeof(int));
        index_.write(reinterpret_cast<char *>(&tail_num), sizeof(int));
        index_.write(reinterpret_cast<char *>(&block_num), sizeof(int));
        index_.close();
    }

    void Insert(key_type key_in, value_type value_in) {
        KeyValuePair in;
        in.value = value_in;
        in.key = key_in;
// 先顺序查找到要插入的块
        Block search_block = FindBlockInsert(in);
        int search_block_num = search_block.my_num_;
// 再二分找到块中要插入的位置
        int insert_place;
        bool if_find_place = false;
        int l = 0;
        int r = search_block.elements_num_ - 1;
        while (l <= r && !if_find_place) {
            int mid = (l + r) / 2;
            if (search_block.value_[mid] == in) {
                insert_place = mid + 1;
                if_find_place = true;
            } else if (search_block.value_[mid] > in)
                r = mid - 1;
            else if (search_block.value_[mid] < in)
                l = mid + 1;
        }
        if (!if_find_place) {
            insert_place = l;
        }
// 执行插入操作
        search_block.elements_num_++;
//        std::cerr<<search_block.elements_num_<< std::endl;
        for (int i = search_block.elements_num_; i > insert_place; i--) {
            search_block.value_[i] = search_block.value_[i - 1];
        }
        search_block.value_[insert_place] = in;
//        index_.open(index_name_);
        if (insert_place == 0) {
            Block temp;
            int temp_num = search_block.prev_num_;
            index_.seekg(head_preserved + sizeof(Block) * (temp_num));
            index_.read(reinterpret_cast<char *>(&temp), sizeof(Block));
            while (temp.elements_num_ == 0 && temp.my_num_ != head_num) {
                temp_num = temp.prev_num_;
                index_.seekg(head_preserved + sizeof(Block) * (temp_num));
                index_.read(reinterpret_cast<char *>(&temp), sizeof(Block));
            }
            temp.next_min_ = in;
            index_.seekp(head_preserved + sizeof(Block) * temp.my_num_);
            index_.write(reinterpret_cast<char *>(&temp), sizeof(Block));
        }
        index_.seekp(head_preserved + sizeof(Block) * search_block_num);
        index_.write(reinterpret_cast<char *>(&search_block), sizeof(Block));
//        index_.close();
// 再检查是否要裂块
        if (search_block.elements_num_ > kMaxNumPerBlock) {
            DivideBlock(search_block_num);
        }
    }

// 裂块函数
    void DivideBlock(int old_num) {
//        index_.open(index_name_);
//        index_.seekg(head_preserved + 1);
        int new_num = -1;

//TODO: 与并块相匹配，外存回收
//        for (int i = 0; i < block_num; ++i) {
//            Block temp;
//            index_.read(reinterpret_cast<char *>(&temp), sizeof(Block));
//            if (!temp.if_occupied) {
//                new_num = i;
//                break;
//            }
//        }
//        if (new_num == -1) {
//            new_num = block_num;
//            block_num++;
//        }
        new_num = block_num;
        block_num++;
        Block new_block(new_num);// 插入新块
        Block next_block;
        Block old_block;
        index_.seekg(head_preserved + sizeof(Block) * old_num);
        index_.read(reinterpret_cast<char *>(&old_block), sizeof(Block));
        index_.seekg(head_preserved + sizeof(Block) * old_block.next_num_);
        index_.read(reinterpret_cast<char *>(&next_block), sizeof(Block));
        new_block.prev_num_ = old_block.my_num_;
        new_block.next_num_ = next_block.my_num_;
        old_block.next_num_ = new_block.my_num_;
        next_block.prev_num_ = new_block.my_num_;
        index_.seekp(head_preserved + sizeof(Block) * old_block.my_num_);
        index_.write(reinterpret_cast<char *>(&old_block), sizeof(Block));
        index_.seekp(head_preserved + sizeof(Block) * new_block.my_num_);
        index_.write(reinterpret_cast<char *>(&new_block), sizeof(Block));
        index_.seekp(head_preserved + sizeof(Block) * next_block.my_num_);
        index_.write(reinterpret_cast<char *>(&next_block), sizeof(Block));
//        index_.close();
// 将原块的一半拷贝到新块上
        CopyBlock(new_num, old_num, kMaxNumPerBlock / 2);
    }

    // 将旧块从下标为start开始复制到新块中
    void CopyBlock(int new_num, int old_num, int start) {
//        index_.open(index_name_);
        Block temp_old;
        Block temp_new(new_num);
        index_.seekg(head_preserved + old_num * sizeof(Block));
        index_.read(reinterpret_cast<char *>(&temp_old), sizeof(Block));
        index_.seekg(head_preserved + new_num * sizeof(Block));
        index_.read(reinterpret_cast<char *>(&temp_new), sizeof(Block));
        for (int i = 0; i < temp_old.elements_num_ - start; ++i) {
            temp_new.value_[i] = temp_old.value_[i + start];
        }
        temp_new.elements_num_ = temp_old.elements_num_ - start;
        temp_old.elements_num_ = start;
        temp_new.next_min_ = temp_old.next_min_;
        temp_old.next_min_ = temp_new.value_[0];
        index_.seekp(head_preserved + old_num * sizeof(Block));
        index_.write(reinterpret_cast<char *>(&temp_old), sizeof(Block));
        index_.seekp(head_preserved + new_num * sizeof(Block));
        index_.write(reinterpret_cast<char *>(&temp_new), sizeof(Block));
//        index_.close();
    }

    Block FindBlockInsert(KeyValuePair in) {
        Block search_block;
        int search_block_num;
        bool if_find_block = false;
        search_block_num = head_num;
        index_.seekg(head_preserved + sizeof(Block) * head_num);
        index_.read(reinterpret_cast<char *>(&search_block), sizeof(Block));
        search_block_num = search_block.next_num_;
        while (search_block_num != tail_num) {
            index_.seekg(head_preserved + sizeof(Block) * search_block_num);
            index_.read(reinterpret_cast<char *>(&search_block), sizeof(Block));
            if (search_block.next_min_ > in)
                break;
            search_block_num = search_block.next_num_;
        }
        return search_block;
    }

    void Delete(key_type key_in, value_type value_in) {
        KeyValuePair in;
        in.key = key_in;
        in.value = value_in;
// 先顺序查找到要删除的块
        Block search_block = FindBlockDelete(in);
        int search_block_num = search_block.my_num_;
// 再二分查找到要删除的位置
        int delete_place;
        bool if_find_place = false;
        int l = 0;
        int r = search_block.elements_num_ - 1;
        while (l <= r && !if_find_place) {
            int mid = (l + r) / 2;
            if (search_block.value_[mid] == in) {
                delete_place = mid;
                if_find_place = true;
            } else if (search_block.value_[mid] > in)
                r = mid - 1;
            else if (search_block.value_[mid] < in)
                l = mid + 1;
        }
        if (!if_find_place) {
            // TODO: 若查找不到，进行错误抛出
//            std::cerr<<"error"<<std::endl;
        }
// 若找到则执行删除操作
        if (if_find_place) {
            search_block.elements_num_--;
            for (int i = delete_place; i < search_block.elements_num_; i++) {
                search_block.value_[i] = search_block.value_[i + 1];
            }
        }

//        index_.open(index_name_);
        index_.seekp(head_preserved + sizeof(Block) * search_block_num);
        index_.write(reinterpret_cast<char *>(&search_block), sizeof(Block));
//        index_.close();
// 检查是否需要并块
        // TODO:
    }


    Block FindBlockDelete(KeyValuePair in) {
        Block search_block;
        int search_block_num;
        bool if_find_block = false;
        search_block_num = head_num;
        index_.seekg(head_preserved + sizeof(Block) * head_num);
        index_.read(reinterpret_cast<char *>(&search_block), sizeof(Block));
        search_block_num = search_block.next_num_;
        while (search_block_num != tail_num) {
            index_.seekg(head_preserved + sizeof(Block) * search_block_num);
            index_.read(reinterpret_cast<char *>(&search_block), sizeof(Block));
            if (search_block.elements_num_ != 0 && search_block.value_[0] <= in &&
                search_block.value_[search_block.elements_num_ - 1] >= in)
                break;
            search_block_num = search_block.next_num_;
        }

//        index_.close();
        return search_block;
    }

// 先写成块内遍历的版本
// TODO当前特为T1评测的输出版本
    void Traverse(std::vector<value_type> &ans, key_type in_key) {
//        index_.open(index_name_);
        Block search_block;
        int search_block_num;
        search_block_num = head_num;
        index_.seekg(head_preserved + sizeof(Block) * search_block_num);
        index_.read(reinterpret_cast<char *>(&search_block), sizeof(Block));
        search_block_num = search_block.next_num_;
//        if (search_block.value_[0].key <= in_key && search_block.next_min_.key >= in_key)
//            TraverseBlock(ans, search_block, in_key);
        while (search_block_num != tail_num) {
            index_.seekg(head_preserved + sizeof(Block) * search_block_num);
            index_.read(reinterpret_cast<char *>(&search_block), sizeof(Block));
            if (search_block.next_num_ != tail_num) {
                if ((search_block.value_[0].key <= in_key && search_block.next_min_.key >= in_key) &&
                    search_block.elements_num_ != 0)
                    TraverseBlock(ans, search_block, in_key);
            } else {
                if (search_block.value_[0].key <= in_key && search_block.elements_num_ != 0)
                    TraverseBlock(ans, search_block, in_key);
            }
            search_block_num = search_block.next_num_;
        }
//        index_.close();
        return;
    }

    void TraverseBlock(std::vector<value_type> &ans, const Block &search_block, key_type in_key) {
        int start, end;
        int l = 0;
        int r = search_block.elements_num_ - 1;
        int mid;
        while (l <= r) {
            mid = l + ((r - l) >> 1);
            if (in_key <= search_block.value_[mid].key)
                r = mid - 1;
            else
                l = mid + 1;
        }
        start = l;
        l = 0;
        r = search_block.elements_num_ - 1;
        while (l <= r) {
            mid = l + ((r - l) >> 1);
            if (in_key < search_block.value_[mid].key)
                r = mid - 1;
            else
                l = mid + 1;
        }
        end = --l;
        for (int i = start; i <= end; ++i) {
            ans.push_back(search_block.value_[i].value);
        }
        return;
    }

    void TraverseAll(std::vector<value_type> &ans) {
        Block search_block;
        int search_block_num;
        search_block_num = head_num;
        index_.seekg(head_preserved + sizeof(Block) * search_block_num);
        index_.read(reinterpret_cast<char *>(&search_block), sizeof(Block));
        search_block_num = search_block.next_num_;
        while (search_block_num != tail_num) {
            index_.seekg(head_preserved + sizeof(Block) * search_block_num);
            index_.read(reinterpret_cast<char *>(&search_block), sizeof(Block));
            TraverseBlockAll(ans, search_block);
            search_block_num = search_block.next_num_;
        }
    }

    void TraverseBlockAll(std::vector<value_type> &ans, const Block &search_block) {
        for (int i = 0; i < search_block.elements_num_; ++i) {
            ans.push_back(search_block.value_[i].value);
        }
    }

    void Get(const key_type &key_in, value_type &ans) {
        Block search_block;
        int search_block_num;
        search_block_num = head_num;
        index_.seekg(head_preserved + sizeof(Block) * search_block_num);
        index_.read(reinterpret_cast<char *>(&search_block), sizeof(Block));
        search_block_num = search_block.next_num_;
        while (search_block_num != tail_num) {
            index_.seekg(head_preserved + sizeof(Block) * search_block_num);
            index_.read(reinterpret_cast<char *>(&search_block), sizeof(Block));
            if (search_block.next_num_ != tail_num) {
                if ((search_block.value_[0].key <= key_in && search_block.next_min_.key > key_in) &&
                    search_block.elements_num_ != 0) {
                    GetBlock(ans, search_block, key_in);
                    break;
                }
            } else {
                if (search_block.value_[0].key <= key_in && search_block.elements_num_ != 0)
                    GetBlock(ans, search_block, key_in);
            }
            search_block_num = search_block.next_num_;
        }
    }

    void GetBlock(value_type &ans, const Block &search_block, key_type key_in) {
        int l = 0;
        int r = search_block.elements_num_ - 1;
        bool if_find = false;
        while (l <= r && !if_find) {
            int mid = l + ((r - l) >> 1);
            if (search_block.value_[mid].key == key_in) {
                if_find = true;
                ans = search_block.value_[mid].value;
            } else if (search_block.value_[mid].key < key_in)
                l = mid + 1;
            else
                r = mid - 1;
        }
        return;
    }

    long GetLastModifyTime() {
        index_.flush();
        struct stat buf;
        FILE *pFile = fopen(index_name_.c_str(), "r");
        int fd = fileno(pFile);
        fstat(fd, &buf);
        long time = buf.st_mtime;
        return time;
    }

    void Save() {
        char temp;
        index_.seekg(0);
        index_ectype_.open(index_ectype_name_, std::ostream::out);
        index_ectype_.seekp(0);
        while (!index_.eof()) {
            index_.read(reinterpret_cast<char *>(&temp), sizeof(char));
            index_ectype_.write(reinterpret_cast<char *>(&temp), sizeof(char));
        }
        index_ectype_.close();


        index_.close();
        index_.open(index_name_);
        last_save_time_ = GetLastModifyTime();
    }

    void Load() {
        char temp;
        index_.close();
        index_.open(index_name_, std::ostream::out);
        index_ectype_.open(index_ectype_name_);
        index_.seekp(0);
        index_ectype_.seekg(0);
        while (!index_ectype_.eof()) {
            index_ectype_.read(reinterpret_cast<char *>(&temp), sizeof(char));
            index_.write(reinterpret_cast<char *>(&temp), sizeof(char));
        }
        index_ectype_.close();
        index_.close();
        index_.open(index_name_);
        last_save_time_ = GetLastModifyTime();
    }

    void AutoSave() {
        long last_modify_time = GetLastModifyTime();
        if (last_modify_time == last_save_time_)
            return;
        else
            Save();
    }
};


#endif //BOOKSTORE_2021_UNROLLED_LINKLIST_H
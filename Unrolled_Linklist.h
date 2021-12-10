//
// Created by 86158 on 2021/12/10.
//
#include <iostream>
#include <fstream>
#include <string>

#ifndef BOOKSTORE_2021_UNROLLED_LINKLIST_H
#define BOOKSTORE_2021_UNROLLED_LINKLIST_H
const int MAX_NUM_PER_BLOCK = 1000;


// 单键版本的块链
template<typename _key_type, typename _value_type>
class Unrolled_linklist_single {
private:
    class Block {
    private:
        struct key_value_pair {// 封装的最小存储单元
            _key_type key;
            _value_type value;
        };

    public:
        key_value_pair first_one;
        key_value_pair last_one;

        Block() {
            next_num = -1,
            prev_num = -1,
            elements_num = 0;
        }

        const int head_preserved = sizeof(int) * 3 + 10;
        const int sizeof_block = MAX_NUM_PER_BLOCK * sizeof(key_value_pair) + head_preserved;
        int elements_num;
        int next_num;
        int prev_num;
    };

    std::fstream _index;
    std::string _index_name;

    const unsigned int head_preserved = sizeof(int) * 3 + 100;
    int head = head_preserved + 1;
    int tail = head_preserved + 1;
    int block_num = 0;

public:
    Unrolled_linklist_single(std::string in) {
        _index_name = in;
        _index.open(_index_name);
        if (!_index) {
            _index.open(_index_name, std::ostream::out);
        } else {
            _index.seekg(0);
        }
        _index.close();
    }

    void copy_block() {

    }

};

#endif //BOOKSTORE_2021_UNROLLED_LINKLIST_H
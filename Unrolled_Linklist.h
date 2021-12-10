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
    struct key_value_pair {// 封装的最小存储单元
        _key_type key;
        _value_type value;
    };


    class Block {
    private:

    public:
        key_value_pair value[1010];
//        key_value_pair first_one;
//        key_value_pair last_one;

        Block() {
            next_num = -1,
            prev_num = -1,
            elements_num = 0;
        }

        Block(int num){
            next_num=-1;
            prev_num=-1;
            elements_num=0;
            my_num=num;
        }

        int elements_num;// 1_based
        int next_num;
        int prev_num;
        int my_num;// 为第几个块，0_based
    };


    std::fstream _index;
    std::string _index_name;

    const unsigned int head_preserved = sizeof(int) * 3 + 100;
    int head = head_preserved;
    int tail = head_preserved;
    int block_num ;

public:
    Unrolled_linklist_single(std::string in) {
        _index_name = in;
        _index.open(_index_name);
        if (!_index) {
            _index.open(_index_name, std::ostream::out);
            head = head_preserved + 1;
            tail = head_preserved + 1;
            block_num = 0;
        } else {
            _index.seekg(0);
            _index.read(reinterpret_cast<char*>(&head), sizeof(int));
            _index.read(reinterpret_cast<char*>(&tail), sizeof(int));
            _index.read(reinterpret_cast<char*>(&block_num), sizeof(int));
        }
        _index.close();
    }

    ~Unrolled_linklist_single(){
        _index.open(_index_name);
        _index.seekp(0);
        _index.write(reinterpret_cast<char*>(&head), sizeof(int));
        _index.write(reinterpret_cast<char*>(&tail), sizeof(int));
        _index.write(reinterpret_cast<char*>(&block_num), sizeof(int));
    }
    void divide_block()

    void copy_block(int new_num, int old_num,int start) {// 从下标为start开始复制到新块中
        _index.open(_index_name);
        Block temp_old;
        Block temp_new(new_num);
        _index.seekg(head_preserved+old_num* sizeof(Block)+1);
        _index.read(reinterpret_cast<char*>(&temp_old), sizeof(Block));
        for(int i=0;i<=temp_old.elements_num-start;++i){
            temp_new.value[i]=temp_old.value[i+start];
        }
        temp_new.elements_num=temp_old.elements_num-start;
        temp_old.elements_num=start;
        _index.seekp(head_preserved+old_num* sizeof(Block)+1);
        _index.write(reinterpret_cast<char*>(&temp_old), sizeof(Block));
        _index.seekp(head_preserved+new_num* sizeof(Block)+1);
        _index.write(reinterpret_cast<char*>(&temp_new), sizeof(Block));
        _index.close();
    }
};

#endif //BOOKSTORE_2021_UNROLLED_LINKLIST_H
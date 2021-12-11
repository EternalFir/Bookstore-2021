//
// Created by 86158 on 2021/12/10.
//
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

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
        bool if_occupied;// 当前块是否已经使用

        // 目前尚未实现并块操作
        // 可以用另一个单链表实现对释放的空间的管理

        key_value_pair value[MAX_NUM_PER_BLOCK + 10];
//        key_value_pair first_one;
//        key_value_pair last_one;

        Block() {
            next_num = -1,
            prev_num = -1,
            elements_num = 0;
        }

        Block(int num) {
            next_num = -1;
            prev_num = -1;
            elements_num = 0;
            my_num = num;
        }

        int elements_num;// 1_based
        int next_num;
        int prev_num;
        int my_num;// 为第几个块，0_based
        key_value_pair next_min;// 后一个块中的最小值
    };


    std::fstream _index;
    std::string _index_name;

    const unsigned int head_preserved = sizeof(int) * 3 + 100;
    int head_num;// 0_based
    int tail_num;
    int block_num;// 已经使用过的块数目

public:
    Unrolled_linklist_single(std::string in) {
        _index_name = in;
        _index.open(_index_name);
        if (!_index) {
            _index.open(_index_name, std::ostream::out);
            head_num = 0;
            tail_num = 0;
            block_num = 0;
        } else {
            _index.seekg(0);
            _index.read(reinterpret_cast<char *>(&head_num), sizeof(int));
            _index.read(reinterpret_cast<char *>(&tail_num), sizeof(int));
            _index.read(reinterpret_cast<char *>(&block_num), sizeof(int));
        }
        _index.close();
    }

    ~Unrolled_linklist_single() {
        _index.open(_index_name);
        _index.seekp(0);
        _index.write(reinterpret_cast<char *>(&head_num), sizeof(int));
        _index.write(reinterpret_cast<char *>(&tail_num), sizeof(int));
        _index.write(reinterpret_cast<char *>(&block_num), sizeof(int));
    }

    void insert(_key_type key_in, _value_type value_in) {
        key_value_pair in;
        in.value = value_in;
        in.key = key_in;
// 先顺序查找到要插入的块
        Block search_block= find_block(in);
        int search_block_num=search_block.my_num;

//        bool if_find_block = false;
//        search_block_num = head_num;
//        _index.seekg(head_preserved + sizeof(Block) * head_num);
//        _index.read(reinterpret_cast<char *>(&search_block), sizeof(Block));
//        if (in.key < search_block.next_min)
//            if_find_block = true;
//        while (search_block_num != tail_num && !if_find_block) {
//            search_block_num = search_block.next_num;
//            _index.seekg(head_preserved + sizeof(Block) * search_block_num);
//            _index.read(reinterpret_cast<char *>(&search_block), sizeof(Block));
//            if (search_block_num == tail_num) {
//                if (in.key >= search_block.value[0].key) {
//                    if_find_block = true;
//                }
//            } else {
//                if (in.key >= search_block.value[0].key && in.key < search_block.next_min)
//                    if_find_block = true;
//            }
//        }

// 再二分找到块中要插入的位置
        int insert_place;
        bool if_find_place = false;
        int l = 0;
        int r = search_block.elements_num - 1;
        while (l <= r && !if_find_place) {
            int mid = (l + r) / 2;
            if (search_block.value[mid].key == in.key) {
                insert_place = mid + 1;
                if_find_place = true;
            } else if (search_block.value[mid].key > in.key)
                r = mid - 1;
            else if (search_block.value[mid].key < in.key)
                l = mid + 1;
        }
        if (!if_find_place) {
            insert_place = l;
        }
// 执行插入操作
        search_block.elements_num++;
        for (int i = search_block.elements_num; i > insert_place; i--) {
            search_block.value[i] = search_block.value[i - 1];
        }
        search_block.value[insert_place] = in;
        _index.open(_index_name);
        if (insert_place == 0) {
            Block temp;
            _index.seekg(head_preserved + sizeof(Block) * (search_block_num - 1));
            _index.read(reinterpret_cast<char *>(&temp), sizeof(Block));
            temp.next_min = in;
            _index.seekp(head_preserved + sizeof(Block) * (search_block_num - 1));
            _index.write(reinterpret_cast<char *>(&temp), sizeof(Block));
        }
        _index.seekp(head_preserved + sizeof(Block) * search_block_num);
        _index.write(reinterpret_cast<char *>(&search_block), sizeof(Block));
        _index.close();
// 再检查是否要裂块
        if (search_block.elements_num > MAX_NUM_PER_BLOCK) {
            divide_block(search_block_num);
        }
    }
// 裂块函数
    void divide_block(int old_num) {
        _index.open(_index_name);
        _index.seekg(head_preserved + 1);
        int new_num = -1;
        Block temp;
        for (int i = 0; i < block_num; ++i) {
            _index.read(reinterpret_cast<char *>(&temp), sizeof(Block));
            if (!temp.if_occupied) {
                new_num = i;
                break;
            }
        }
        if (new_num == -1) {
            new_num = block_num;
            block_num++;
        }
        _index.close();
// 将原块的一半拷贝到新块上
        copy_block(new_num,old_num,MAX_NUM_PER_BLOCK/2);
    }

    // 将旧块从下标为start开始复制到新块中
    void copy_block(int new_num, int old_num, int start) {
        _index.open(_index_name);
        Block temp_old;
        Block temp_new(new_num);
        _index.seekg(head_preserved + old_num * sizeof(Block) + 1);
        _index.read(reinterpret_cast<char *>(&temp_old), sizeof(Block));
        for (int i = 0; i <= temp_old.elements_num - start; ++i) {
            temp_new.value[i] = temp_old.value[i + start];
        }
        temp_new.elements_num = temp_old.elements_num - start;
        temp_old.elements_num = start;
        temp_new.next_min = temp_old.next_min;
        temp_old.next_min = temp_new.value[0];
        _index.seekp(head_preserved + old_num * sizeof(Block) + 1);
        _index.write(reinterpret_cast<char *>(&temp_old), sizeof(Block));
        _index.seekp(head_preserved + new_num * sizeof(Block) + 1);
        _index.write(reinterpret_cast<char *>(&temp_new), sizeof(Block));
        _index.close();
    }

    void delete_ (_key_type key_in, _value_type value_in){
        key_value_pair in;
        in.key=key_in;
        in.value=value_in;
// 先顺序查找到要删除的块
        Block search_block= find_block(in);
        int search_block_num=search_block.my_num;
// 再二分查找到要删除的位置
        int delete_place;
        bool if_find_place = false;
        int l = 0;
        int r = search_block.elements_num - 1;
        while (l <= r && !if_find_place) {
            int mid = (l + r) / 2;
            if (search_block.value[mid].key == in.key) {
                delete_place = mid;
                if_find_place = true;
            } else if (search_block.value[mid].key > in.key)
                r = mid - 1;
            else if (search_block.value[mid].key < in.key)
                l = mid + 1;
        }
        if (!if_find_place) {
            // TODO: 若查找不到，进行错误抛出
        }
// 执行删除操作
        search_block.elements_num--;
        for (int i=delete_place;i<search_block_num;i++) {
            search_block.value[i]=search_block.value[i+1];
        }
        _index.open(_index_name);
        _index.seekp(head_preserved+ sizeof(Block)*search_block_num);
        _index.write(reinterpret_cast<char*>(&search_block), sizeof(Block));
        _index.close();
// 检查是否需要并块
        // TODO:
    }

    Block find_block(key_value_pair in){
        _index.open(_index_name);
        Block search_block;
        int search_block_num;
        bool if_find_block = false;
        search_block_num = head_num;
        _index.seekg(head_preserved + sizeof(Block) * head_num);
        _index.read(reinterpret_cast<char *>(&search_block), sizeof(Block));
        if (in.key < search_block.next_min)
            if_find_block = true;
        while (search_block_num != tail_num && !if_find_block) {
            search_block_num = search_block.next_num;
            _index.seekg(head_preserved + sizeof(Block) * search_block_num);
            _index.read(reinterpret_cast<char *>(&search_block), sizeof(Block));
            if (search_block_num == tail_num) {
                if (in.key >= search_block.value[0].key) {
                    if_find_block = true;
                }
            } else {
                if (in.key >= search_block.value[0].key && in.key < search_block.next_min)
                    if_find_block = true;
            }
        }
        _index.close();
        return search_block;
    }
};

#endif //BOOKSTORE_2021_UNROLLED_LINKLIST_H
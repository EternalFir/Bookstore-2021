#include <iostream>
#include "Unrolled_Linklist.h"
#include <string>
#include <cstring>
#include <vector>
struct Char{
    char value[65];

    bool operator==(const Char &rhs) const {
        if(strcmp(value,rhs.value)==0)
            return true;
        else
            return false;
    }

    bool operator!=(const Char &rhs) const {
        if(strcmp(value,rhs.value)!=0)
            return true;
        else
            return false;
    }

    bool operator<(const Char &rhs) const {
        return strcmp(value,rhs.value)<0;
    }

    bool operator>(const Char &rhs) const {
        return strcmp(value,rhs.value)>0;
    }

    bool operator<=(const Char &rhs) const {
        return strcmp(value,rhs.value)<=0;
    }

    bool operator>=(const Char &rhs) const {
        return strcmp(value,rhs.value)>=0;
    }
};

int main() {
    int n;
    Char index;
    int value;
    std::string order;
    std::cin >> n;
    Unrolled_linklist_single<Char, int> store("storage");
    for (int i = 0; i < n; i++) {
        std::cin >> order;
        if (order == "insert") {
            std::cin >> index.value;
            std::cin >> value;
            store.insert(index, value);
        }
        else if(order=="delete"){
            std::cin>>index.value;
            std::cin>>value;
            store.delete_(index,value);
        } else if(order=="find"){
            std::cin>>index.value;
            std::vector<Unrolled_linklist_single<Char,int>::key_value_pair> out;
            store.traverse(out,index);
            if(out.size()==0)
                std::cout<<"null";
            else{
                for (int j = 0; j < out.size(); ++j) {
                    std::cout<<out[i].value<<' ';
                }
            }
            std::cout<<std::endl;
        }
    }
    return 0;
}

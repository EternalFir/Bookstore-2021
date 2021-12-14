#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "Unrolled_Linklist.h"
#include <cstdio>
struct Char{
    char value_[65];

    bool operator==(const Char &rhs) const {
        if(strcmp(value_,rhs.value_)==0)
            return true;
        else
            return false;
    }

    bool operator!=(const Char &rhs) const {
        if(strcmp(value_,rhs.value_)!=0)
            return true;
        else
            return false;
    }

    bool operator<(const Char &rhs) const {
        return strcmp(value_,rhs.value_)<0;
    }

    bool operator>(const Char &rhs) const {
        return strcmp(value_,rhs.value_)>0;
    }

    bool operator<=(const Char &rhs) const {
        return strcmp(value_,rhs.value_)<=0;
    }

    bool operator>=(const Char &rhs) const {
        return strcmp(value_,rhs.value_)>=0;
    }
};

int main() {
//#ifndef ONLINE_JUDGE
//    freopen("5in","r",stdin);
//    freopen("!output.txt","w",stdout);
//#endif
    int n;
    Char index;
    int value;
    std::string order;
    std::cin >> n;
    UnrolledLinklist<Char, int> store("storage");
    for (int i = 0; i < n; i++) {
        std::cin >> order;
        if (order == "insert") {
            std::cin >> index.value_;
            std::cin >> value;
            store.Insert(index, value);
        }
        else if(order=="delete"){
            std::cin>>index.value_;
            std::cin>>value;
            store.Delete(index,value);
        } else if(order=="find"){
            std::cin>>index.value_;
            std::vector<int> out;
            store.Traverse(out,index);
            if(out.size()==0)
                std::cout<<"null";
            else{
                for (int j = 0; j < out.size(); ++j) {
                    std::cout<<out[j]<<' ';
                }
            }
            std::cout<<std::endl;
        }
    }
    return 0;
}

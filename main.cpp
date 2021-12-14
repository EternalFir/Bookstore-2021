#include <iostream>
#include "Unrolled_Linklist.h"
#include <string>
#include <cstring>
#include <vector>
#include <cstdio>
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
//#ifndef ONLINE_JUDGE
//    freopen("5in","r",stdin);
//    freopen("!output.txt","w",stdout);
//#endif
    int n;

    int roll_num=0;

    Char index;
    int value;
    std::string order;
    std::cin >> n;
    Unrolled_linklist<Char, int> store("storage");
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

            roll_num++;

            std::vector<int> out;
            store.traverse(out,index);
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

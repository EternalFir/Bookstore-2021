#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "Unrolled_Linklist.h"
#include <cstdio>
#include "Accounts.h"
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
void  Initialization();

int main() {
//#ifndef ONLINE_JUDGE
//    freopen("5in","r",stdin);
//    freopen("!output.txt","w",stdout);
//#endif

    return 0;
}

void Initialization(){

}
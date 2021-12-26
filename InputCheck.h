//
// Created by 86158 on 2021/12/26.
//

#ifndef BOOKSTORE_2021_INPUTCHECK_H
#define BOOKSTORE_2021_INPUTCHECK_H

#include <iostream>
#include <string>

void CheckType0(std::string& input){
    if(input.length()>1024)
        throw std::string("Invalid\n");
    
}


#endif //BOOKSTORE_2021_INPUTCHECK_H

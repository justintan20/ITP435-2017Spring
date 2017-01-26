//
//  Dictionary.cpp
//  password-mac
//
//  Created by Justin Tan on 1/25/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "Dictionary.h"
#include "Timer.h"
#include "Sha1.cpp"
Dictionary::Dictionary(std::string fileName){
    std::ifstream file(fileName);
    if(file.is_open()){
        mHash_pass.rehash(100000);
        Timer timer;
        timer.start();
        while(file.eof() == false){
            std::string* line;
            std::getline(file, *line);
            unsigned char hash[20];
            sha1::calc(line, line->length(), hash);
            char hex_str[41];
            mHash_pass[hex_str] = *line;
        }
        double elapsed = timer.getElapsed();
        std::cout << elapsed << std::endl;
    }
    else{
        
    }
}

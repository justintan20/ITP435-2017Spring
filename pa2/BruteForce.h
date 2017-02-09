//
//  BruteForce.hpp
//  password-mac
//
//  Created by Justin Yong Jia Tan on 2/2/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>
#include <vector>

class BruteForce
{
public:
    //default constructor
    BruteForce();
    //actual brute force algorithim
    void BruteForceHelper(std::vector<std::pair<std::string,std::string>>& unsolved, int start, int end);
    //converts counting machines to strings
    std::string convert1(int countMachine1) const;
    std::string convert2(int countMachine2[2]) const;
    std::string convert3(int countMachine3[3]) const;
    std::string convert4(int countMachine4[4]) const;
    //helper for converting
    char convertElement(int input) const;
};

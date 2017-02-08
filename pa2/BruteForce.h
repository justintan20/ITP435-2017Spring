//
//  BruteForce.hpp
//  password-mac
//
//  Created by Justin Tan on 2/2/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>
#include <vector>

class BruteForce
{
private:
    //counting machines
    int mCountMachine4[4];
    int mCountMachine3[3];
    int mCountMachine2[2];
    int mCountMachine1;
    
public:
    //default constructor
    BruteForce();
//    std::string BruteForceParallel(std::string& unsolved);
    //actual brute force algorithim
    void BruteForceHelper(std::vector<std::pair<std::string,std::string>>& unsolved, int start, int end);
    //converts counting machines to strings
    std::string convert1() const;
    std::string convert2() const;
    std::string convert3() const;
    std::string convert4() const;
    //helper for converting
    char convertElement(int input) const;
};

//
//  Dictionary.hpp
//  password-mac
//
//  Created by Justin Tan on 1/25/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>
#include <unordered_map>

class Dictionary{
private:
    std::string mFileName;
    std::unordered_map<std::string, std::string> mHash_pass;
    
public:
    Dictionary(std::string fileName);
};

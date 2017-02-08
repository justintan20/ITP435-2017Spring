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

class Dictionary
{
private:
    std::unordered_map<std::string, std::string> mHashPass;
    
public:
    //dictionary constructor with given file name
    Dictionary(const std::string& fileName);
    //finds the hex hash in the dictionary and returns hash with solution
    std::pair<std::string, std::string> findHash(const std::string& hash) const;
};

//
//  Decrypt.hpp
//  password-mac
//
//  Created by Justin Yong Jia Tan on 2/1/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <vector>
#include <string>
#include "Dictionary.h"

class Decrypt{
private:
    std::vector<std::pair<std::string, std::string>> mDecrypted;
public:
    //decrypts passwords first with dictionary then with brute force
    Decrypt(const std::string& passFileName, const Dictionary& dictionary);
};

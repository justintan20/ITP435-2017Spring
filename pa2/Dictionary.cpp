//
//  Dictionary.cpp
//  password-mac
//
//  Created by Justin Yong Jia Tan on 1/25/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//
#include <fstream>
#include <iostream>
#include <string>
#include "Dictionary.h"
#include "Timer.h"
#include "Sha1.h"

Dictionary::Dictionary(const std::string& fileName)
{
    //load file and hash data
    std::ifstream file(fileName);
    if(file.is_open())
    {
        std::cout << "Dictionary file opened...";
        mHashPass.rehash(100000);
        Timer timer;
        timer.start();
        while(file.eof() == false)
        {
            std::string line;
            std::getline(file, line);
            unsigned char hash[20];
            sha1::calc(line.c_str(), line.length(), hash);
            char hex_str[41];
            sha1::toHexString(hash, hex_str);
            mHashPass[hex_str] = line;
        }
        std::cout << "data hashed and loaded!" << std::endl;
        float fElapsed = timer.getElapsed();
        std::cout << "Seconds used: " << fElapsed << std::endl;
    }
    else
    {
        std::cout << "File not found!" << std::endl;
    }
}

std::pair<std::string, std::string> Dictionary::findHash(const std::string& hash) const
{
    //check if hash is in dictionary, if not set result as ??
    if(mHashPass.find(hash) == mHashPass.end())
    {
        std::pair<std::string, std::string> output = std::make_pair(hash, "??");
        return output;
    }
    else
    {
        return *mHashPass.find(hash);
    }
}

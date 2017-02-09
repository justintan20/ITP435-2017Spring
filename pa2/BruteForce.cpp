//
//  BruteForce.cpp
//  password-mac
//
//  Created by Justin Yong Jia Tan on 2/2/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "BruteForce.h"
#include "Sha1.h"
#include <iostream>

BruteForce::BruteForce()
{
    //initialize counting machines
    
}

void BruteForce::BruteForceHelper(std::vector<std::pair<std::string, std::string>>& unsolved, int start, int end)
{
    //set counting machines
    int countMachine4[4];
    int countMachine3[3];
    int countMachine2[2];
    int countMachine1;
    countMachine1 = start;
    countMachine2[0] = start;
    countMachine3[0] = start;
    countMachine4[0] = start;
    countMachine4[0] = 0;
    countMachine4[1] = 0;
    countMachine4[2] = 0;
    countMachine4[3] = 0;
    countMachine1 = 0;
    countMachine2[0] = 0;
    countMachine2[1] = 0;
    countMachine3[0] = 0;
    countMachine3[1] = 0;
    countMachine3[2] = 0;
    //helper booleans
    bool loopEnd1 = false;
    bool loopEnd2 = false;
    bool loopEnd3 = false;
    bool loopEnd4 = false;
    //check for length 1
    while(loopEnd1 == false)
    {
        //convert and hash
        std::string convertedStr = convert1(countMachine1);
        unsigned char hash[20];
        sha1::calc(convertedStr.c_str(), 1, hash);
        char hex_str[41];
        sha1::toHexString(hash, hex_str);
        for (int i = 0; i < unsolved.size(); i++)
        {
            if(hex_str == unsolved[i].first)
            {
                unsolved[i].second = convertedStr;
            }
        }
        countMachine1++;
        if(countMachine1 == end + 1)
        {
            loopEnd1 = true;
        }
    }
    //check for length 2
    while(loopEnd2 == false)
    {
        std::string convertedStr = convert2(countMachine2);
        unsigned char hash[20];
        sha1::calc(convertedStr.c_str(), 2, hash);
        char hex_str[41];
        sha1::toHexString(hash, hex_str);
        for(int i = 0; i < unsolved.size(); i++)
        {
            if(hex_str == unsolved[i].first)
            {
                unsolved[i].second = convertedStr;
            }
        }
        countMachine2[1]++;
        if(countMachine2[1] == 36)
        {
            countMachine2[1] = 0;
            countMachine2[0]++;
        }
        if(countMachine2[0] == end + 1)
        {
            loopEnd2 = true;
        }
    }
    //check for length 3
    while(loopEnd3 == false)
    {
        std::string convertedStr = convert3(countMachine3);
        unsigned char hash[20];
        sha1::calc(convertedStr.c_str(), 3, hash);
        char hex_str[41];
        sha1::toHexString(hash, hex_str);
        for(int i = 0; i < unsolved.size(); i++)
        {
            if(hex_str == unsolved[i].first)
            {
                unsolved[i].second = convertedStr;
            }
        }
        countMachine3[2]++;
        if(countMachine3[2] == 36)
        {
            countMachine3[2] = 0;
            countMachine3[1]++;
        }
        if(countMachine3[1] == 36)
        {
            countMachine3[1] = 0;
            countMachine3[0]++;
        }
        if(countMachine3[0] == end + 1)
        {
            loopEnd3 = true;
        }
    }
    //check for length 4
    while(loopEnd4 == false)
    {
        std::string convertedStr = convert4(countMachine4);
        unsigned char hash[20];
        sha1::calc(convertedStr.c_str(), 4, hash);
        char hex_str[41];
        sha1::toHexString(hash, hex_str);
        for (int i = 0; i < unsolved.size(); i++)
        {
            if(hex_str == unsolved[i].first)
            {
                unsolved[i].second = convertedStr;
            }
        }
        countMachine4[3]++;
        if(countMachine4[3] == 36)
        {
            countMachine4[3] = 0;
            countMachine4[2]++;
        }
        if(countMachine4[2] == 36)
        {
            countMachine4[2] = 0;
            countMachine4[1]++;
        }
        if(countMachine4[1] == 36)
        {
            countMachine4[1] = 0;
            countMachine4[0]++;
        }
        if(countMachine4[0] == end + 1)
        {
            //reaches end of check, set to ?? if still unknown
            loopEnd4 = true;
        }
    }
}

std::string BruteForce::convert1(int countMachine1) const
{
    char output[2];
    output[0] = convertElement(countMachine1);
    output[1] = '\0';
    return output;
}

std::string BruteForce::convert2(int countMachine2[2]) const
{
    char output[3];
    for(int i = 0; i < 2; i++)
    {
        output[i] = convertElement(countMachine2[i]);
    }
    output[2] = '\0';
    return output;
}

std::string BruteForce::convert3(int countMachine3[3]) const
{
    char output[4];
    for(int i = 0; i < 3; i++)
    {
        output[i] = convertElement(countMachine3[i]);
    }
    output[3] = '\0';
    return output;
}

std::string BruteForce::convert4(int countMachine4[4]) const
{
    char output[5];
    for(int i = 0; i < 4; i++)
    {
        output[i] = convertElement(countMachine4[i]);
    }
    output[4] = '\0';
    return output;
}

char BruteForce::convertElement(int input) const
{
    //cover 36 values: lowercase a-z and 0-9
    switch (input) {
        case 0:
            return 'a';
            break;
        case 1:
            return 'b';
            break;
        case 2:
            return 'c';
            break;
        case 3:
            return 'd';
            break;
        case 4:
            return 'e';
            break;
        case 5:
            return 'f';
            break;
        case 6:
            return 'g';
            break;
        case 7:
            return 'h';
            break;
        case 8:
            return 'i';
            break;
        case 9:
            return 'j';
            break;
        case 10:
            return 'k';
            break;
        case 11:
            return 'l';
            break;
        case 12:
            return 'm';
            break;
        case 13:
            return 'n';
            break;
        case 14:
            return 'o';
            break;
        case 15:
            return 'p';
            break;
        case 16:
            return 'q';
            break;
        case 17:
            return 'r';
            break;
        case 18:
            return 's';
            break;
        case 19:
            return 't';
            break;
        case 20:
            return 'u';
            break;
        case 21:
            return 'v';
            break;
        case 22:
            return 'w';
            break;
        case 23:
            return 'x';
            break;
        case 24:
            return 'y';
            break;
        case 25:
            return 'z';
            break;
        case 26:
            return '0';
            break;
        case 27:
            return '1';
            break;
        case 28:
            return '2';
            break;
        case 29:
            return '3';
            break;
        case 30:
            return '4';
            break;
        case 31:
            return '5';
            break;
        case 32:
            return '6';
            break;
        case 33:
            return '7';
            break;
        case 34:
            return '8';
            break;
        case 35:
            return '9';
            break;
        default:
            return '\0';
            break;
    }
}

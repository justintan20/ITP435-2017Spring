//
//  DNAData.cpp
//  dnawx-mac
//
//  Created by Justin Tan on 3/6/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "DNAData.h"
#include "Exceptions.h"
#include <fstream>

DNAData::DNAData(const std::string& fileName)
{
    std::ifstream::pos_type size;
    char* memblock;
    std::ifstream file(fileName,std::ios::in|std::ios::ate);
    //check if file exists
    if(file.is_open())
    {
        //load file
        size = file.tellg();
        memblock = new char [static_cast<unsigned int>(size)];
        mSequence.reserve(size);
        file.seekg(0, std::ios::beg);
        std::getline(file, mHeader);
        //check if header is right format
        if(mHeader[0] != '>')
        {
            throw FileLoadExcept();
        }
        else
        {
            mHeader = mHeader.substr(1);
            char add;
            while(file.get(add))
            {
                //check if data is valid
                if(add != 'T' && add != 'C' && add != 'A' && add != 'G')
                {
                    continue;
                }
                else
                {
                    mSequence += add;
                }
            }
        }
        file.close();
    }
    else
    {
        throw FileLoadExcept();
    }
}

//Getters
std::string DNAData::GetSequence() const
{
    return mSequence;
}

std::string DNAData::GetHeader() const
{
    return mHeader;
}

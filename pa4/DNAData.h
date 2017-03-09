//
//  DNAData.hpp
//  dnawx-mac
//
//  Created by Justin Tan on 3/6/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>

class DNAData
{
private:
    //member variables
    std::string mHeader;
    std::string mSequence;
public:
    //load data with constructor
    DNAData(const std::string& fileName);
    std::string GetSequence() const;
    std::string GetHeader() const;
};

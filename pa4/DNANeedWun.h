//
//  DNANeedWun.hpp
//  dnawx-mac
//
//  Created by Justin Tan on 3/8/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>
#include "DNAData.h"

enum Direction : char
{
    ABOVE_LEFT,
    LEFT,
    ABOVE
};

class DNANeedWun
{
public:
    DNANeedWun(const std::string& fileName1, const std::string& fileName2);
    void Run();
    void WriteResults(const std::string& fileName) const;
    
private:
    std::shared_ptr<DNAData> mData1;
    std::shared_ptr<DNAData> mData2;
    std::string mOptimal1;
    std::string mOptimal2;
    int mFinalScore;
};

//
//  DNATranslate.hpp
//  dnawx-mac
//
//  Created by Justin Tan on 3/6/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>

class DNATranslate
{
public:
    DNATranslate();
    void Translate(const std::string& sequence);
    void SetData(std::shared_ptr<DNATranslate> other);
    void SetHeader(const std::string& header);
    std::string GetHeader() const;
    int GetA() const;
    int GetC() const;
    int GetD() const;
    int GetE() const;
    int GetF() const;
    int GetG() const;
    int GetH() const;
    int GetI() const;
    int GetK() const;
    int GetL() const;
    int GetM() const;
    int GetN() const;
    int GetP() const;
    int GetQ() const;
    int GetR() const;
    int GetS() const;
    int GetT() const;
    int GetV() const;
    int GetW() const;
    int GetY() const;
    int GetTotal() const;
private:
    const int mStateMachine[24][4] =
    {
        //T,C,A,G
        //0
        {0,0,1,0},
        //1
        {2,0,1,0},
        //2
        {0,0,1,'M'},
        //3
        {4,9,14,19},
        //4
        {5,6,7,8},
        //5
        {'F','F','L','L'},
        //6
        {'S','S','S','S'},
        //7
        {'Y','Y',0,0},
        //8
        {'C','C',0,'W'},
        //9
        {10,11,12,13},
        //10
        {'L','L','L','L'},
        //11
        {'P','P','P','P'},
        //12
        {'H','H','Q','Q'},
        //13
        {'R','R','R','R'},
        //14
        {15,16,17,18},
        //15
        {'I','I','I','M'},
        //16
        {'T','T','T','T'},
        //17
        {'N','N','K','K'},
        //18
        {'S','S','R','R'},
        //19
        {20,21,22,23},
        //20
        {'V','V','V','V'},
        //21
        {'A','A','A','A'},
        //22
        {'D','D','E','E'},
        //23
        {'G','G','G','G'}
    };
    int mNumF = 0;
    int mNumL = 0;
    int mNumI = 0;
    int mNumM = 0;
    int mNumV = 0;
    int mNumS = 0;
    int mNumP = 0;
    int mNumT = 0;
    int mNumA = 0;
    int mNumY = 0;
    int mNumH = 0;
    int mNumQ = 0;
    int mNumN = 0;
    int mNumK = 0;
    int mNumD = 0;
    int mNumE = 0;
    int mNumC = 0;
    int mNumW = 0;
    int mNumR = 0;
    int mNumG = 0;
    int mNumTotal = 0;
    std::string mHeader;
};

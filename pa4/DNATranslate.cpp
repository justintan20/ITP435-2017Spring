//
//  DNATranslate.cpp
//  dnawx-mac
//
//  Created by Justin Tan on 3/6/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "DNATranslate.h"

DNATranslate::DNATranslate()
{
    
}

void DNATranslate::SetHeader(const std::string& header)
{
    mHeader = header;
}

std::string DNATranslate::GetHeader() const
{
    return mHeader;
}

void DNATranslate::SetData(std::shared_ptr<DNATranslate> other)
{
    //set all data the same and inputed data
    mNumTotal = other->GetTotal();
    mNumA = other->GetA();
    mNumC = other->GetC();
    mNumD = other->GetD();
    mNumE = other->GetE();
    mNumF = other->GetF();
    mNumG = other->GetG();
    mNumH = other->GetH();
    mNumI = other->GetI();
    mNumK = other->GetK();
    mNumL = other->GetL();
    mNumM = other->GetM();
    mNumN = other->GetN();
    mNumP = other->GetP();
    mNumQ = other->GetQ();
    mNumR = other->GetR();
    mNumS = other->GetS();
    mNumT = other->GetT();
    mNumV = other->GetV();
    mNumW = other->GetW();
    mNumY = other->GetY();
    mHeader = other->GetHeader();
}

void DNATranslate::Translate(const std::string& sequence)
{
    int currentState = 0;
    for(int i = 0; i < sequence.size(); i++)
    {
        int index = 0;
        char letter = sequence.at(i);
        //check which letter
        switch (letter)
        {
            case 'T':
                index = 0;
                break;
            case 'C':
                index = 1;
                break;
            case 'A':
                index = 2;
                break;
            case 'G':
                index = 3;
                break;
        }
        //get corresponding state
        currentState = mStateMachine[currentState][index];
        //if is amino acid, update values and go to state 3
        switch (currentState)
        {
            case 'F':
                mNumF++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'L':
                mNumL++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'I':
                mNumI++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'M':
                mNumM++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'V':
                mNumV++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'S':
                mNumS++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'P':
                mNumP++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'T':
                mNumT++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'A':
                mNumA++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'Y':
                mNumY++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'H':
                mNumH++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'Q':
                mNumQ++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'N':
                mNumN++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'K':
                mNumK++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'D':
                mNumD++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'E':
                mNumE++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'C':
                mNumC++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'W':
                mNumW++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'R':
                mNumR++;
                mNumTotal++;
                currentState = 3;
                break;
            case 'G':
                mNumG++;
                mNumTotal++;
                currentState = 3;
                break;
            default:
                break;
        }
    }
}

int DNATranslate::GetA() const
{
    return mNumA;
}

int DNATranslate::GetC() const
{
    return mNumC;
}

int DNATranslate::GetD() const
{
    return mNumD;
}

int DNATranslate::GetE() const
{
    return mNumE;
}

int DNATranslate::GetF() const
{
    return mNumF;
}

int DNATranslate::GetG() const
{
    return mNumG;
}

int DNATranslate::GetH() const
{
    return mNumH;
}

int DNATranslate::GetI() const
{
    return mNumI;
}

int DNATranslate::GetK() const
{
    return mNumK;
}

int DNATranslate::GetL() const
{
    return mNumL;
}

int DNATranslate::GetM() const
{
    return mNumM;
}

int DNATranslate::GetY() const
{
    return mNumY;
}

int DNATranslate::GetN() const
{
    return mNumN;
}

int DNATranslate::GetP() const
{
    return mNumP;
}

int DNATranslate::GetQ() const
{
    return mNumQ;
}

int DNATranslate::GetR() const
{
    return mNumR;
}

int DNATranslate::GetS() const
{
    return mNumS;
}

int DNATranslate::GetT() const
{
    return mNumT;
}

int DNATranslate::GetV() const
{
    return mNumV;
}

int DNATranslate::GetW() const
{
    return mNumW;
}

int DNATranslate::GetTotal() const
{
    return mNumTotal;
}

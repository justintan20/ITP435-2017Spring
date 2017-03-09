//
//  DNANeedWun.cpp
//  dnawx-mac
//
//  Created by Justin Tan on 3/8/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "DNANeedWun.h"
#include <vector>
#include <fstream>
#include <wx/busyinfo.h>

DNANeedWun::DNANeedWun(const std::string& fileName1, const std::string& fileName2)
{
    mData1 = std::make_shared<DNAData>(fileName1);
    mData2 = std::make_shared<DNAData>(fileName2);
    mOptimal1 = "";
    mOptimal2 = "";
}

void DNANeedWun::Run()
{
    std::string sequence1 = mData1->GetSequence();
    std::string sequence2 = mData2->GetSequence();
    unsigned long length1 = sequence1.size() + 1;
    unsigned long length2 = sequence2.size() + 1;

    std::vector<std::vector<short>> scoreGrid(length2, std::vector<short>(length1));
    std::vector<std::vector<Direction>> pathGrid(length2, std::vector<Direction>(length1));;
    scoreGrid[0][0] = 0;
    for(int i = 1; i < length1; i++)
    {
        scoreGrid[0][i] = -1*i;
        pathGrid[0][i] = LEFT;
    }
    for(int j = 1; j < length2; j++)
    {
        scoreGrid[j][0] = -1*j;
        pathGrid[j][0] = ABOVE;
    }
    for(int row = 1; row < length2; row++)
    {
        for(int col = 1; col < length1; col++)
        {
            int highestScore;
            int matchScore;
            int leftScore;
            int aboveScore;
            int leftAboveScore;
            if(sequence1.at(col - 1) == sequence2.at(row - 1))
            {
                matchScore = 1;
            }
            else
            {
                matchScore = -1;
            }
            leftAboveScore = scoreGrid[row - 1][col -1] + matchScore;
            leftScore = scoreGrid[row][col - 1] - 1;
            aboveScore = scoreGrid[row - 1][col] - 1;
            if(leftAboveScore >= leftScore && leftAboveScore >= aboveScore)
            {
                highestScore = leftAboveScore;
                pathGrid[row][col] = ABOVE_LEFT;
            }
            else if(leftScore >= aboveScore)
            {
                highestScore = leftScore;
                pathGrid[row][col] = LEFT;
            }
            else
            {
                highestScore = aboveScore;
                pathGrid[row][col] = ABOVE;
            }
            scoreGrid[row][col] = highestScore;
        }
    }
    mFinalScore = scoreGrid[length2 - 1][length1 - 1];
    unsigned long currRow = length2 - 1;
    unsigned long currCol = length1 - 1;
    while(currRow != 0 || currCol != 0)
    {
        if(pathGrid[currRow][currCol] == ABOVE_LEFT)
        {
            mOptimal1 += sequence1.at(currCol - 1);
            mOptimal2 += sequence2.at(currRow - 1);
            currRow--;
            currCol--;
        }
        else if (pathGrid[currRow][currCol] == LEFT)
        {
            mOptimal1 += sequence1.at(currCol - 1);
            mOptimal2 += '_';
            currCol--;
        }
        else
        {
            mOptimal1 += '_';
            mOptimal2 += sequence2.at(currRow - 1);
            currRow--;
        }
    }
    std::reverse(mOptimal1.begin(),mOptimal1.end());
    std::reverse(mOptimal2.begin(),mOptimal2.end());
}

void DNANeedWun::WriteResults(const std::string& fileName) const
{
    std::ofstream file(fileName);
    if(file.is_open())
    {
        file << "A: " << mData1->GetHeader() << "\nB: " << mData2->GetHeader() << "\nScore: " << mFinalScore << "\n" << std::endl;
        std::string linkString = "";
        for(int i = 0; i < mOptimal1.size(); i++)
        {
            if((mOptimal1.at(i) == mOptimal2.at(i)) && mOptimal1.at(i) != '_')
            {
                linkString += '|';
            }
            else
            {
                linkString += ' ';
            }
        }
        int lines = static_cast<int>(mOptimal1.size()) / 70;
        int remainder = mOptimal1.size() % 70;
        for(int j = 0; j < lines; j++)
        {
            file << mOptimal1.substr(70*j,70) << "\n" << linkString.substr(70*j,70) << "\n" << mOptimal2.substr(70*j,70) << "\n" << std::endl;
        }
        if(remainder != 0)
        {
            file << mOptimal1.substr(lines * 70) << "\n" << linkString.substr(lines * 70) << "\n" << mOptimal2.substr(lines * 70) << "\n" << std::endl;
        }
    }
}

//
//  Decrypt.cpp
//  password-mac
//
//  Created by Justin Tan on 2/1/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Decrypt.h"
#include "BruteForce.h"
#include "Timer.h"
#include <fstream>
#include <iostream>
#include <map>
#include <tbb/parallel_invoke.h>

Decrypt::Decrypt(const std::string& passFileName, const Dictionary& dictionary)
{
    //load file data
    std::ifstream passFile(passFileName);
    std::ofstream decryptedFile("pass_solved.txt");
    if(passFile.is_open())
    {
        std::cout << "Password file opened..." << std::endl;;
        while(passFile.eof() == false)
        {
            std::string line;
            std::getline(passFile,line);
            //decrypt using dictionary
            mDecrypted.push_back(dictionary.findHash(line));
        }
        passFile.close();
        std::cout << "Simple decryption complete!" << std::endl;
        //split data into solved and unsolved
        std::map<int, std::pair<std::string, std::string>> solved;
        std::vector<std::pair<std::string, std::string>> unsolved;
        for(int i = 0; i < mDecrypted.size(); i++)
        {
            solved[i] = mDecrypted[i];
            if(mDecrypted[i].second == "??")
            {
                unsolved.push_back(mDecrypted[i]);
            }
        }
        //brute force unsolved
        Timer time;
        time.start();
        BruteForce bruteForceObject;
//        tbb::parallel_invoke(
//                             [&bruteForceObject, &unsolved] { bruteForceObject.BruteForceHelper(unsolved, 0, 3);},
//                             [&bruteForceObject, &unsolved] { bruteForceObject.BruteForceHelper(unsolved, 4, 7);},
//                             [&bruteForceObject, &unsolved] { bruteForceObject.BruteForceHelper(unsolved, 8, 11);},
//                             [&bruteForceObject, &unsolved] { bruteForceObject.BruteForceHelper(unsolved, 12, 15);},
//                             [&bruteForceObject, &unsolved] { bruteForceObject.BruteForceHelper(unsolved, 16, 19);},
//                             [&bruteForceObject, &unsolved] { bruteForceObject.BruteForceHelper(unsolved, 20, 23);},
//                             [&bruteForceObject, &unsolved] { bruteForceObject.BruteForceHelper(unsolved, 24, 27);},
//                             [&bruteForceObject, &unsolved] { bruteForceObject.BruteForceHelper(unsolved, 28, 31);},
//                             [&bruteForceObject, &unsolved] { bruteForceObject.BruteForceHelper(unsolved, 32, 35);}
//                             );
        bruteForceObject.BruteForceHelper(unsolved, 0, 35);
        std::cout << "Brute force time: " << time.getElapsed() << std::endl;
        int unsolvedIndex = 0;
        for (int i = 0; i < solved.size(); i++)
        {
            if(solved[i].second == "??")
            {
                solved[i] = unsolved[unsolvedIndex];
                unsolvedIndex++;
            }
            decryptedFile << solved[i].first << "," << solved[i].second << '\n';
        }
    }
    else
    {
        std::cout << "File not found!" << std::endl;
    }
}

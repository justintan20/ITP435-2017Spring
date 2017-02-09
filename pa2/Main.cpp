// Main.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <string>
#include "Sha1.h"
#include "Dictionary.h"
#include "Decrypt.h"

int main(int argc, char* argv[])
{
    //test hashing
    if(argc == 2)
    {
        unsigned char hash[20];
        sha1::calc(argv[1], strlen(argv[1]), hash);
        char hex_str[41];
        sha1::toHexString(hash, hex_str);
        std::cout << hex_str << std::endl;
    }
    //decrypt
    else if(argc == 3)
    {
        const std::string dictFileName = argv[1];
        const std::string passFileName = argv[2];
        Dictionary dictFile(dictFileName);
        Decrypt passFile(passFileName, dictFile);

    }
    return 0;
}


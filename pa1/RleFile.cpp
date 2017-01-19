#include "RLEFile.h"
#include "RleData.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "FileSystem.h"

void RleFile::CreateArchive(const std::string& source){
    //initialize variables to store size and data
    std::ifstream::pos_type size;
    char* memblock;
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    //check if file is open
    if (file.is_open())
    {
        std::cout << "File opened: " << source << std::endl;
        size = file.tellg(); // Save the size of the file
        memblock = new char [static_cast<unsigned int>(size)];
        file.seekg (0, std::ios::beg); // Seek back to start of file
        file.read (memblock, size);
        file.close();
        // File data has now been loaded into memblock array
        //set mHeader variables accordingly
        mHeader.fileNameLength = source.length();
        mHeader.fileSize = static_cast<unsigned int>(size);
        mHeader.sig[0] = 'R';
        mHeader.sig[1] = 'L';
        mHeader.sig[2] = 'E';
        mHeader.sig[3] = '\x01';
        mHeader.fileName = source;
        //compress data
        mData.Compress(memblock, mHeader.fileSize);
        std::cout << "Compression successful!" << std::endl;
        //clean up
        delete[] memblock;
        double percent = mData.mSize / (double)size * 100;
        std::cout << "Resulted in a " << percent << "% compression." << std::endl;
        std::ofstream arc(source + ".rl1", std::ios::out|std::ios::binary|std::ios::trunc);
        if (arc.is_open())
        {
            // Use arc.write function to write data here
            //first write header info
            arc.write(mHeader.sig, 4);
            arc.write(reinterpret_cast<char*>(&(mHeader.fileSize)), 4);
            arc.write(reinterpret_cast<char*>(&(mHeader.fileNameLength)), 1);
            arc.write(mHeader.fileName.c_str(),mHeader.fileNameLength);
            //then write actual compressed data
            arc.write(mData.mData, mData.mSize);
        }
    }
    //tell user if file did not open
    else{
        std::cout << "File does not exist!" << std::endl;
    }
}

void RleFile::ExtractArchive(const std::string& source){
    //initialize variables
    std::ifstream::pos_type size;
    char* memblock;
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    //check if file is open
    if (file.is_open()){
        std::cout << "File opened: " << source << std::endl;
        size = file.tellg(); // Save the size of the file
        memblock = new char [static_cast<unsigned int>(size)];
        file.seekg (0, std::ios::beg); // Seek back to start of file
        file.read (memblock, size);
        file.close();
        // File data has now been loaded into memblock array
        //check header to see if file is valid
        if (memblock[0] != 'R' || memblock[1] != 'L' || memblock[2] != 'E' || memblock[3] != '\x01'){
            std::cout << "Invalid archive!" << std::endl;
        }
        else{
            //set mHeader variables
            mHeader.sig[0] = 'R';
            mHeader.sig[1] = 'L';
            mHeader.sig[2] = 'E';
            mHeader.sig[3] = '\x01';
            mHeader.fileSize = *(reinterpret_cast<int*>(&memblock[4]));
            mHeader.fileNameLength = memblock[8];
            for (int i = 0; i < mHeader.fileNameLength; i++){
                mHeader.fileName = mHeader.fileName + memblock[9 + i];
            }
            //created new char array storing compressed data
            char* compressed = new char[static_cast<unsigned int>(size) - 9 - mHeader.fileNameLength];
            //store into helper char array
            for (int j = 0; j < static_cast<unsigned int>(size) - 9 - mHeader.fileNameLength; j++){
                compressed[j] = memblock[9 + mHeader.fileNameLength + j];
            }
            //clean up original
            delete[] memblock;
            //decompress data
            mData.Decompress(compressed, static_cast<unsigned int>(size) - 9 - mHeader.fileNameLength, mHeader.fileSize);
            //clean up
            delete[] compressed;
            std::cout << "Decompression complete!" << std::endl;
            std::ofstream arc(mHeader.fileName, std::ios::out|std::ios::binary|std::ios::trunc);
            if (arc.is_open())
            {
                // Use arc.write function to write data here
                arc.write(mData.mData, mData.mSize);
            }
        }
    }
    //tell user if file did not open
    else{
        std::cout << "File does not exist!" << std::endl;
    }
}

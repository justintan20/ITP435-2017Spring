#include "RLEFile.h"
#include "RleData.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "FileSystem.h"

void RleFile::CreateArchive(const std::string& source)
{
	// TODO
    // Requires <fstream>
    std::ifstream::pos_type size;
    char* memblock;
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        std::cout << "File opened." << std::endl;
        size = file.tellg(); // Save the size of the file
        memblock = new char [static_cast<unsigned int>(size)];
        file.seekg (0, std::ios::beg); // Seek back to start of file
        file.read (memblock, size);
        file.close();
        // File data has now been loaded into memblock array
        unsigned int sizeBefore = static_cast<unsigned int>(size);
        mHeader.fileNameLength = source.length();
        mHeader.fileSize = sizeBefore;
        mHeader.sig[0] = 'R';
        mHeader.sig[1] = 'L';
        mHeader.sig[2] = 'E';
        mHeader.sig[3] = '\x01';
        mHeader.fileName = source;
        mData.Compress(memblock, sizeBefore);
        double percent = mData.mSize / (double)size * 100;
        std::cout << "Resulted in a " << percent << "% compression." << std::endl;
        std::ofstream arc(source + ".rl1", std::ios::out|std::ios::binary|std::ios::trunc);
        if (arc.is_open())
        {
            // Use arc.write function to write data here
            arc.write(mHeader.sig, 4);
            arc.write(reinterpret_cast<char*>(&(mHeader.fileSize)), 4);
            arc.write(reinterpret_cast<char*>(&(mHeader.fileNameLength)), 1);
            arc.write(mHeader.fileName.c_str(),mHeader.fileNameLength);
            arc.write(mData.mData, mData.mSize);
        }
        // Make sure to clean up!
        delete[] memblock;
    }
}

void RleFile::ExtractArchive(const std::string& source)
{
	// TODO
    // Requires <fstream>
    mData.~RleData();
    std::ifstream::pos_type size;
    char* memblock;
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        std::cout << "File opened." << std::endl;
        size = file.tellg(); // Save the size of the file
        memblock = new char [static_cast<unsigned int>(size)];
        file.seekg (0, std::ios::beg); // Seek back to start of file
        file.read (memblock, size);
        file.close();
        std::cout << "File read." << std::endl;
        // File data has now been loaded into memblock array
        if (memblock[0] != 'R' || memblock[1] != 'L' || memblock[2] != 'E' || memblock[3] != '\x01'){
            std::cout << "Invalid archive!" << std::endl;
        }
        else{
            std::cout << "valid data" <<std::endl;
            mHeader.sig[0] = 'R';
            mHeader.sig[1] = 'L';
            mHeader.sig[2] = 'E';
            mHeader.sig[3] = '\x01';
            mHeader.fileSize = *(reinterpret_cast<int*>(&memblock[4]));
            std::cout << "filesize loaded." << mHeader.fileSize << std::endl;
            mHeader.fileNameLength = memblock[8];
            std::cout << "filename length found." << std::endl;
            for (int i = 0; i < mHeader.fileNameLength; i++){
                mHeader.fileName = mHeader.fileName + memblock[9 + i];
            }
            std::cout << "filename loaded. " << mHeader.fileName << std::endl;
            char* compressed = new char[static_cast<unsigned int>(size) - 9 - mHeader.fileNameLength];
            for (int j = 0; j < static_cast<unsigned int>(size) - 9 - mHeader.fileNameLength; j++){
                compressed[j] = memblock[9 + mHeader.fileNameLength + j];
            }
            std::cout << "compressed data found." << std::endl;
            mData.Decompress(compressed, static_cast<unsigned int>(size) - 9 - mHeader.fileNameLength, mHeader.fileSize);
            std::cout << "decompress complete." << std::endl;
            std::ofstream arc(mHeader.fileName, std::ios::out|std::ios::binary|std::ios::trunc);
            if (arc.is_open())
            {
                // Use arc.write function to write data here
                arc.write(mData.mData, mData.mSize);
            }
        }
        // Make sure to clean up!
        delete[] memblock;
    }
}

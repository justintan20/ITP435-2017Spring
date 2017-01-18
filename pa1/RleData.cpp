#include "RleData.h"
#include <iostream>
#include <math.h>

void RleData::Compress(const char* input, size_t inSize)
{
    // TODO
    delete[] mData;
    mSize = 0;
    mData = new char[inSize * 2];
    int num = 1;
    int diff = 1;
    int index = 0;
    char* differ = new char[2 * inSize];
    for (int i = 1; i < inSize; i++)
    {
        char prev = input[i - 1];
        char next = input[i + 1];
        if (input[i] == prev)
        {
            num++;
            if (num == 128){
                mData[index] = 127;
                index++;
                mData[index] = prev;
                index++;
                num = 1;
            }
            else if (i == inSize - 1){
                mData[index] = num;
                index++;
                mData[index] = prev;
                index++;
                num = 1;
            }
        }
        else if (input[i] != prev)
        {
            if (num == 1){
                if ((input[i] != next) && (i != inSize - 1)){
                    differ[diff - 1] = prev;
                    diff++;
                }
                else if (i == inSize - 1){
                    differ[diff - 1] = prev;
                    diff++;
                    differ[diff - 1] = input[i];
                    mData[index] = diff * (-1);
                    index++;
                    for (int j = 0; j < diff; j++){
                        mData[index] = differ[j];
                        index++;
                    }
                    diff = 1;
                }
                else if (input[i] == next){
                    differ[diff - 1] = prev;
                    if (diff > 1){
                        mData[index] = diff * (-1);
                    }
                    else{
                        mData[index] = diff;
                    }
                    index++;
                    for (int j = 0; j < diff; j++){
                        mData[index] = differ[j];
                        index++;
                    }
                    diff = 1;
                }
            }
            else if (num > 1){
                mData[index] = num;
                index++;
                mData[index] = prev;
                index++;
                num = 1;
            }
        }
    }
    mSize = index;
}

void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
    // TODO
    delete[] mData;
    mSize = 0;
    mData = new char[outSize];
    int index = 0;
    int outIndex = 0;
    while (index < inSize){
        if (input[index] > 0){
            for (int i = 0; i < input[index]; i++){
                mData[outIndex] = input[index + 1];
                mSize++;
                outIndex++;
            }
            index = index + 2;
        }
        else if (input[index] < 0){
            for (int i = 0; i < input[index] * -1; i++){
                mData[outIndex] = input[index + i + 1];
                mSize++;
                outIndex++;
            }
            index = index + input[index] * -1 + 1;
        }
    }
}

std::ostream& operator<< (std::ostream& stream, const RleData& rhs)
{
    for (int i = 0; i < rhs.mSize; i++)
    {
        stream << rhs.mData[i];
    }
    return stream;
}

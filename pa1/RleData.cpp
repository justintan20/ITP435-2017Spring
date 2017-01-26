#include "RleData.h"
#include <iostream>
#include <math.h>

void RleData::Compress(const char* input, size_t inSize){
    //initialize variables
    delete[] mData;
    mSize = 0;
    mData = new char[inSize * 2];
    //helper variables
    int num = 1;
    int diff = 1;
    //this is used to help input the alternating chars
    char* differ = new char[2 * inSize];
    //loop through entire char array
    for (int i = 1; i < inSize; i++){
        //previous and next char helper variables
        char prev = input[i - 1];
        char next = input[i + 1];
        char curr = input[i];

        //case where the two chars are the same
        if (curr == prev){
            //increment num of same chars
            num++;
            //if reaches max run size
            if (num == MaxRunSize() + 1){
                //split and continue
                mData[mSize] = MaxRunSize();
                mSize++;
                mData[mSize] = prev;
                mSize++;
                //reset num
                num = 1;
            }
            //if reaches end
            else if (i == inSize - 1){
                mData[mSize] = num;
                mSize++;
                mData[mSize] = prev;
                mSize++;
            }
        }
        //case where two chars are not the same
        else if (curr != prev){
            //if it is single outstanding char
            if (num == 1){
                //if the next one is still not equal and also not the end of the input
                if ((curr != next) && (i != inSize - 1)){
                    //add char to the helper char array and increment number of different chars
                    differ[diff - 1] = prev;
                    diff++;
                    //if exceeds max run size
                    if (diff == MaxRunSize() + 1){
                        mData[mSize] = MaxRunSize() * (-1);
                        mSize++;
                        for (int j = 0; j < MaxRunSize(); j++){
                            mData[mSize] = differ[j];
                            mSize++;
                        }
                        diff = 1;
                    }
                }
                //if reaches end
                else if (i == inSize - 1){
                    //add the last two to the helper array
                    differ[diff - 1] = prev;
                    diff++;
                    differ[diff - 1] = curr;
                    //first add negative number indication
                    mData[mSize] = diff * (-1);
                    mSize++;
                    //copy data in helper array to mData
                    for (int j = 0; j < diff; j++){
                        mData[mSize] = differ[j];
                        mSize++;
                    }
                    delete[] differ;
                    diff = 1;
                }
                //if the next one and the current one are the same
                else if (curr == next){
                    //add previous to helper array
                    differ[diff - 1] = prev;
                    //if more than 1 alternating, use negative number
                    if (diff > 1){
                        mData[mSize] = diff * (-1);
                    }
                    //otherwise, use 1
                    else{
                        mData[mSize] = diff;
                    }
                    mSize++;
                    //now copy data from helper array to mData
                    for (int j = 0; j < diff; j++){
                        mData[mSize] = differ[j];
                        mSize++;
                    }
                    diff = 1;
                }
            }
            //if more than one the same from before
            else if (num > 1){
                //add them to mData
                mData[mSize] = num;
                mSize++;
                mData[mSize] = prev;
                mSize++;
                //reset
                num = 1;
            }
        }
    }
}

void RleData::Decompress(const char* input, size_t inSize, size_t outSize){
    //initialize variables accordingly
    delete[] mData;
    mSize = 0;
    mData = new char[outSize];
    //helper variable
    int index = 0;
    //loop until the end
    while (index < inSize){
        //if the number is positive
        if (input[index] > 0){
            //copy one char the number of times
            for (int i = 0; i < input[index]; i++){
                mData[mSize] = input[index + 1];
                mSize++;
            }
            //jump to next number, which is 2 spots away
            index = index + 2;
        }
        //if the number is negative
        else if (input[index] < 0){
            //copy the number of chars into mData
            for (int i = 0; i < input[index] * -1; i++){
                mData[mSize] = input[index + i + 1];
                mSize++;
            }
            //jump to next number, which is adding the absolute value of the number and adding 1
            index = index + input[index] * -1 + 1;
        }
    }
}

std::ostream& operator<< (std::ostream& stream, const RleData& rhs){
    for (int i = 0; i < rhs.mSize; i++)
    {
        stream << rhs.mData[i];
    }
    return stream;
}

// zompiler.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include "node.h"
#include <fstream>

extern int g_LineNumber;
extern NBlock* g_MainBlock;

extern int zompilerparse();
extern FILE* zompilerin;

//function to optimize gotos
int optimize(CodeContext& context, std::pair<int, int> pair)
{
    //check if links
    if(context.mGotos.find(pair.second) != context.mGotos.end())
    {
        std::pair<int, int> pair2;
        pair2.first = pair.first;
        pair2.second = context.mGotos.find(pair.second)->second;
        //use recursion to find till end
        return optimize(context, pair2);
    }
    else
    {
        return pair.second;
    }
}

int main(int argc, char* argv[])
{
	// Read input from the first command line parameter
	zompilerin = fopen(argv[1], "r");
	// Start the parse
	zompilerparse();
	// CodeGen from g_MainBlock
    CodeContext myContext;
    if(g_MainBlock != nullptr)
    {
        g_MainBlock->CodeGen(myContext);
    }
	// Close the file stream
	fclose(zompilerin);
    //optimize all gotos
    for(auto i : myContext.mGotos)
    {
        if(myContext.mGotos.find(i.second) != myContext.mGotos.end())
        {
            int opitmizedGoto = optimize(myContext, i);
            myContext.mGotos[i.first] = opitmizedGoto;
            myContext.mOps[i.first - 1] = "goto," + std::to_string(opitmizedGoto);
        }
    }
    //output file
    std::ofstream output("out.zom");
    for(int i = 0; i < myContext.mOps.size(); i++)
    {
        output << myContext.mOps[i] << "\n";
    }
	return 0;
}

void zompilererror(const char* s)
{
	std::cout << s << " on line " << g_LineNumber << std::endl;
}

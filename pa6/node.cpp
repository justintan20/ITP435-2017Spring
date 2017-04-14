#include "node.h"
#include <sstream>

void NBlock::AddStatement(NStatement* statement)
{
	mStatements.push_back(statement);
}

void NBlock::CodeGen(CodeContext& context) const
{
	// TODO: Loop through statements in list and code gen them
    for(auto const& i : mStatements)
    {
        i->CodeGen(context);
    }
    if(mbMainBlock)
    {
        context.mOps.push_back("goto,1");
        context.mGotos[static_cast<int>(context.mOps.size())] = 1;
    }
}

NNumeric::NNumeric(std::string& value)
{
	mvalue = std::atoi(value.c_str());
}

NRotate::NRotate(NNumeric* dir)
	: mDir(dir)
{
}

void NRotate::CodeGen(CodeContext& context) const
{
	if (mDir->mvalue == 0)
	{
		context.mOps.push_back("rotate,0");
	}
	else if (mDir->mvalue == 1)
	{
		context.mOps.push_back("rotate,1");
	}
}

//forward
NForward::NForward()
{
    
}

void NForward::CodeGen(CodeContext& context) const
{
    context.mOps.push_back("forward");
}

//if else
NIfElse::NIfElse(NBoolean* boolean, NBlock* ifBlock, NBlock* elseBlock)
{
    mBool = boolean;
    mIfBlock = ifBlock;
    mElseBlock = elseBlock;
}

void NIfElse::CodeGen(CodeContext& context) const
{
    mBool->CodeGen(context);
    unsigned int index1 = static_cast<int>(context.mOps.size());
    context.mOps.push_back("je,");
    mElseBlock->CodeGen(context);
    unsigned int index2 = static_cast<int>(context.mOps.size());
    context.mOps.push_back("goto,");
    mIfBlock->CodeGen(context);
    context.mOps[index1] += std::to_string(index2 + 2);
    context.mOps[index2] += std::to_string(context.mOps.size() + 1);
    context.mGotos[index2 + 1] = static_cast<int>(context.mOps.size() + 1);
}

//attack
NAttack::NAttack()
{
    
}

void NAttack::CodeGen(CodeContext& context) const
{
    context.mOps.push_back("attack");
}

//ranged_attack
NRangedAttack::NRangedAttack()
{
    
}

void NRangedAttack::CodeGen(CodeContext& context) const
{
    context.mOps.push_back("ranged_attack");
}

//is_human
NIsHuman::NIsHuman(NNumeric* num) : mNum(num)
{
    
}

void NIsHuman::CodeGen(CodeContext& context) const
{
    if(mNum->mvalue == 1)
    {
        context.mOps.push_back("test_human,1");
    }
    else if(mNum->mvalue == 2)
    {
        context.mOps.push_back("test_human,2");
    }
}

//is_passable
NIsPassable::NIsPassable()
{
    
}

void NIsPassable::CodeGen(CodeContext& context) const
{
    context.mOps.push_back("test_passable");
}

//is_random
NIsRandom::NIsRandom()
{
    
}

void NIsRandom::CodeGen(CodeContext& context) const
{
    context.mOps.push_back("test_random");
}

//is_zombie
NIsZombie::NIsZombie(NNumeric* num) : mNum(num)
{
    
}

void NIsZombie::CodeGen(CodeContext& context) const
{
    if(mNum->mvalue == 1)
    {
        context.mOps.push_back("test_zombie,1");
    }
    else if(mNum->mvalue == 2)
    {
        context.mOps.push_back("test_zombie,2");
    }
}

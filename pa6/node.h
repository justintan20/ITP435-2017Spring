#pragma once

#include <list>
#include <vector>
#include <string>
#include <map>

struct CodeContext
{
	std::vector<std::string> mOps;
    std::map<int, int> mGotos;
};

class Node
{
public:
	virtual void CodeGen(CodeContext& context) const = 0;
};

class NNumeric : public Node
{
public:
	NNumeric(std::string& value);
	int mvalue;
	virtual void CodeGen(CodeContext& context) const override { }
};

//NStatement
class NStatement : public Node
{
};

class NRotate : public NStatement
{
public:
	NRotate(NNumeric* dir);
	virtual void CodeGen(CodeContext& context) const override;
private:
	NNumeric* mDir;
};

class NForward : public NStatement
{
public:
    NForward();
    virtual void CodeGen(CodeContext& context) const override;
};

class NAttack : public NStatement
{
public:
    NAttack();
    virtual void CodeGen(CodeContext& context) const override;
};

class NRangedAttack : public NStatement
{
public:
    NRangedAttack();
    virtual void CodeGen(CodeContext& context) const override;
};

//NBoolean
class NBoolean : public Node
{
};

class NIsHuman : public NBoolean
{
public:
    NIsHuman(NNumeric* num);
    virtual void CodeGen(CodeContext& context) const override;
private:
    NNumeric* mNum;
};

class NIsPassable : public NBoolean
{
public:
    NIsPassable();
    virtual void CodeGen(CodeContext& context) const override;
};

class NIsRandom : public NBoolean
{
public:
    NIsRandom();
    virtual void CodeGen(CodeContext& context) const override;
};

class NIsZombie : public NBoolean
{
public:
    NIsZombie(NNumeric* num);
    virtual void CodeGen(CodeContext& context) const override;
private:
    NNumeric* mNum;
};

class NBlock : public Node
{
public:
	NBlock() { mbMainBlock = false; }
	virtual void CodeGen(CodeContext& context) const override;
	void AddStatement(NStatement* statement);
	void SetMainBlock() { mbMainBlock = true; }
private:
	std::list<NStatement*> mStatements;
	bool mbMainBlock;
};

class NIfElse : public NStatement
{
public:
    NIfElse(NBoolean* boolean, NBlock* ifBlock, NBlock* elseBlock);
    virtual void CodeGen(CodeContext& context) const override;
private:
    NBoolean* mBool;
    NBlock* mIfBlock;
    NBlock* mElseBlock;
};

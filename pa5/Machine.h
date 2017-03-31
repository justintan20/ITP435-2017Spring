#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "Op.h"
#include "Exceptions.h"
#include <fstream>
#include <string>

// Defines state data accessible by the machine and ops
struct MachineState
{
	// Friend Machine so BindState works
	template <typename MachineTraits>
	friend class Machine;

	enum Facing { UP, RIGHT, DOWN, LEFT };
	MachineState()
		: mProgramCounter(1)
		, mActionsTaken(0)
		, mFacing(UP)
		, mTest(false)
        , mX(0)
        , mY(0)
	{ }

	~MachineState()
	{
	}

	// Active line number in behavior program
	int mProgramCounter;
	// Number of actions taken this turn
	int mActionsTaken;
	// Current facing of this character
	Facing mFacing;
	// Test flag for branches
	bool mTest;
    int mX;
    int mY;

	int GetActionsPerTurn() const noexcept { return mActionsPerTurn; }
	bool GetInfect() const noexcept { return mInfectOnAttack; }
private:
	// Data which is set by the traits
	int mActionsPerTurn;
	bool mInfectOnAttack;
};

// Describes the machine which processes ops.
// Different policies dictate behavior possible for machine.
template <typename MachineTraits>
class Machine
{
public:
	// Load in all the ops for this machine from the specified file
	void LoadMachine(const std::string& filename);

	// Given the state, binds the trait parameters to it
	void BindState(MachineState& state);

	// Take a turn using this logic for the passed in state
	void TakeTurn(MachineState& state);

	// Destructor
	~Machine();
private:
	std::vector<std::shared_ptr<Op>> mOps;
};

template <typename MachineTraits>
void Machine<MachineTraits>::LoadMachine(const std::string& filename)
{
	// TEMP CODE: Add your parsing code here!
	mOps.clear();
    std::ifstream file(filename);
    if(file.is_open())
    {
        while(!file.eof())
        {
            std::string line;
            std::getline(file, line);
            std::string opStr;
            std::string actualLine;
            if(line.find(";") != std::string::npos)
            {
                int endIndex = static_cast<int>(line.find(";"));
                actualLine = line.substr(0,endIndex);
                if(actualLine.find(" ") != std::string::npos)
                {
                    int space = static_cast<int>(actualLine.find(" "));
                    actualLine = actualLine.substr(0,space);
                }
            }
            else
            {
                actualLine = line;
            }
            int num;
            if(actualLine.find(",") != std::string::npos)
            {
                int index = static_cast<int>(actualLine.find(","));
                opStr = actualLine.substr(0,index);
                num = std::stoi(line.substr(index + 1, actualLine.size() - index - 1));
            }
            else
            {
                opStr = actualLine;
            }
            if(opStr.compare("rotate") == 0)
            {
                mOps.push_back(std::make_shared<OpRotate>(num));
            }
            else if(opStr.compare("forward") == 0)
            {
                mOps.push_back(std::make_shared<OpForward>());
            }
            else if(opStr.compare("test_wall") == 0)
            {
                mOps.push_back(std::make_shared<OpTestWall>());
            }
            else if(opStr.compare("je") == 0)
            {
                mOps.push_back(std::make_shared<OpJe>(num));
            }
            else if(opStr.compare("goto") == 0)
            {
                mOps.push_back(std::make_shared<OpGoto>(num));
            }
            else if(opStr.compare("test_random") == 0)
            {
                mOps.push_back(std::make_shared<OpTestRandom>());
            }
            else if(opStr.compare("test_zombie") == 0)
            {
                mOps.push_back(std::make_shared<OpTestZombie>(num));
            }
            else if(opStr.compare("test_passable") == 0)
            {
                mOps.push_back(std::make_shared<OpTestPassable>());
            }
        }
    }
//	mOps.push_back(std::make_shared<OpRotate>(0));
//	mOps.push_back(std::make_shared<OpRotate>(0));
//	mOps.push_back(std::make_shared<OpRotate>(1));
//	mOps.push_back(std::make_shared<OpGoto>(1));
	// END TEMP CODE
    
}

template <typename MachineTraits>
void Machine<MachineTraits>::BindState(MachineState& state)
{
	state.mActionsPerTurn = MachineTraits::ACTIONS_PER_TURN;
	state.mInfectOnAttack = MachineTraits::INFECT_ON_ATTACK;
}

template <typename MachineTraits>
void Machine<MachineTraits>::TakeTurn(MachineState& state)
{
	std::cout << "TAKING TURN" << std::endl;
	state.mActionsTaken = 0;
	while (state.mActionsTaken < MachineTraits::ACTIONS_PER_TURN)
	{
		mOps.at(state.mProgramCounter - 1)->Execute(state);
	}
}

template <typename MachineTraits>
Machine<MachineTraits>::~Machine()
{
	mOps.clear();
}

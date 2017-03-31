#include "Op.h"
#include "Machine.h"
#include <iostream>
#include "Exceptions.h"
#include "ZomWorld.h"

// Output state information for debugging purposes
void Op::DebugOutput(MachineState& state)
{
	std::cout << state.mProgramCounter << ":" << mOpName << "," << mParam << std::endl;
    std::cout << "X: " << state.mX << "      Y: " << state.mY << std::endl;
}

void OpRotate::Execute(MachineState& state)
{
	DebugOutput(state);
	switch (state.mFacing)
	{
	case (MachineState::UP) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::RIGHT;
		}
		else
		{
			state.mFacing = MachineState::LEFT;
		}
		break;
	case (MachineState::RIGHT) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::DOWN;
		}
		else
		{
			state.mFacing = MachineState::UP;
		}
		break;
	case (MachineState::DOWN) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::LEFT;
		}
		else
		{
			state.mFacing = MachineState::RIGHT;
		}
		break;
	default:
	case (MachineState::LEFT) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::UP;
		}
		else
		{
			state.mFacing = MachineState::DOWN;
		}
		break;
	}

//	std::cout << "Now facing ";
//	switch (state.mFacing)
//	{
//	case (MachineState::UP):
//		std::cout << "UP";
//		break;
//	case (MachineState::RIGHT):
//		std::cout << "RIGHT";
//		break;
//	case (MachineState::DOWN):
//		std::cout << "DOWN";
//		break;
//	case (MachineState::LEFT):
//		std::cout << "LEFT";
//		break;
//	}
//	std::cout << std::endl;
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpGoto::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter = mParam;
}

void OpForward::Execute(MachineState& state)
{
    DebugOutput(state);
    int currRow = state.mY / 30;
    int currCol = state.mY / 30;
    int original = ZomWorld::get().GetGrid(currRow, currCol);
    switch (state.mFacing)
    {
        case (MachineState::UP) :
            if(state.mY != 0){
                state.mY -= 30;
                ZomWorld::get().SetGrid(currRow, currCol, 0);
                ZomWorld::get().SetGrid(currRow - 1, currCol, original);
            }
            break;
        case (MachineState::RIGHT) :
            if(state.mX != 570){
                state.mX += 30;
                ZomWorld::get().SetGrid(currRow, currCol, 0);
                ZomWorld::get().SetGrid(currRow, currCol + 1, original);
            }
            break;
        case (MachineState::DOWN) :
            if(state.mY != 570){
                state.mY += 30;
                ZomWorld::get().SetGrid(currRow, currCol, 0);
                ZomWorld::get().SetGrid(currRow + 1, currCol, original);
            }
            break;
        case (MachineState::LEFT) :
            if(state.mX != 0){
                state.mX -= 30;
                ZomWorld::get().SetGrid(currRow, currCol, 0);
                ZomWorld::get().SetGrid(currRow, currCol - 1, original);
            }
            break;
    }
    state.mProgramCounter++;
    state.mActionsTaken++;
}

void OpTestWall::Execute(MachineState& state)
{
    DebugOutput(state);
    switch (state.mFacing)
    {
        case (MachineState::UP) :
            if(state.mY == 0){
                state.mTest = true;
            }
            break;
        case (MachineState::RIGHT) :
            if(state.mX == 570){
                state.mTest = true;
            }
            break;
        case (MachineState::DOWN) :
            if(state.mY == 570){
                state.mTest = true;
            }
            break;
        case (MachineState::LEFT) :
            if(state.mX == 0){
                state.mTest = true;
            }
            break;
    }
    state.mProgramCounter++;
}

void OpJe::Execute(MachineState& state)
{
    DebugOutput(state);
    if(state.mTest)
    {
        state.mProgramCounter = mParam;
        state.mTest = false;
    }
    else
    {
        state.mProgramCounter++;
    }
}

void OpTestRandom::Execute(MachineState& state)
{
    DebugOutput(state);
    state.mTest = rand() % 2;
    state.mProgramCounter++;
}

void OpTestZombie::Execute(MachineState& state)
{
    DebugOutput(state);
    int currRow = state.mY / 30;
    int currCol = state.mX / 30;
    switch (state.mFacing)
    {
        case (MachineState::UP) :
            if(currRow!=0){
                if(ZomWorld::get().GetGrid(currRow - mParam, currCol) == 1)
                {
                    state.mTest = true;
                }
            }
            break;
        case (MachineState::RIGHT) :
            if(currCol!=19){
                if(ZomWorld::get().GetGrid(currRow, currCol + mParam) == 1)
                {
                    state.mTest = true;
                }
            }
            break;
        case (MachineState::DOWN) :
            if(currRow!=19){
                if(ZomWorld::get().GetGrid(currRow + mParam, currCol) == 1)
                {
                    state.mTest = true;
                }
            }
            break;
        case (MachineState::LEFT) :
            if(currCol!=0){
                if(ZomWorld::get().GetGrid(currRow, currCol - mParam) == 1)
                {
                    state.mTest = true;
                }
            }
            break;
    }
    state.mProgramCounter++;
}

void OpTestPassable::Execute(MachineState& state)
{
    DebugOutput(state);
    int currRow = state.mY / 30;
    int currCol = state.mX / 30;
    switch (state.mFacing)
    {
        case (MachineState::UP) :
            if(state.mY != 0)
            {
                if(ZomWorld::get().GetGrid(currRow - 1, currCol) == 0)
                {
                    state.mTest = true;
                }
            }
            break;
        case (MachineState::RIGHT) :
            if(state.mX != 570)
            {
                if(ZomWorld::get().GetGrid(currRow, currCol + 1) == 0)
                {
                    state.mTest = true;
                }
            }
            break;
        case (MachineState::DOWN) :
            if(state.mY != 570)
            {
                if(ZomWorld::get().GetGrid(currRow + 1, currCol) == 0)
                {
                    state.mTest = true;
                }
            }
            break;
        case (MachineState::LEFT) :
            if(state.mX != 0)
            {
                if(ZomWorld::get().GetGrid(currRow, currCol - 1) == 0)
                {
                    state.mTest = true;
                }
            }
            break;
    }
    state.mProgramCounter++;
}

void OpRangedAttack::Execute(MachineState& state)
{
    DebugOutput(state);
}






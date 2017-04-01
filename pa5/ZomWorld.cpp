//
//  ZomWorld.cpp
//  zombiewx-mac
//
//  Created by Justin Tan on 3/29/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "ZomWorld.h"

void ZomWorld::NewWorld()
{
    //clear everything
    mHumans.clear();
    mZombies.clear();
    mHumanStates.clear();
    mZombieStates.clear();
    //set grid unoccupied everywhere
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            mGrid[i][j] = 0;
        }
    }
}

void ZomWorld::AddZombie(std::shared_ptr<Machine<ZombieTraits>> zombie)
{
    MachineState zombieState;
    //create random location
    int row = rand() % 20;
    int col = rand() % 20;
    //check if already occupied
    while(mGrid[row][col] != 0)
    {
        row = rand()%20;
        col = rand()%20;
    }
    //set coordinates
    int x = col * 30;
    int y = row * 30;
    zombieState.mX = x;
    zombieState.mY = y;
    //set random facing direction
    int facing = rand()%4;
    switch (facing) {
        case 0:
            zombieState.mFacing = MachineState::UP;
            break;
        case 1:
            zombieState.mFacing = MachineState::DOWN;
            break;
        case 2:
            zombieState.mFacing = MachineState::LEFT;
            break;
        case 3:
            zombieState.mFacing = MachineState::RIGHT;
            break;
    }
    //bind and add
    zombie->BindState(zombieState);
    mZombies.push_back(zombie);
    mZombieStates.push_back(std::make_shared<MachineState>(zombieState));
    //set occupied
    mGrid[row][col] = 1;
}

//similar to implementation of AddZombie
void ZomWorld::AddHuman(std::shared_ptr<Machine<HumanTraits>> human)
{
    MachineState humanState;
    int row = rand() % 20;
    int col = rand() % 20;
    while(mGrid[row][col] != 0)
    {
        row = rand()%20;
        col = rand()%20;
    }
    int x = col * 30;
    int y = row * 30;
    humanState.mX = x;
    humanState.mY = y;
    int facing = rand()%4;
    switch (facing) {
        case 0:
            humanState.mFacing = MachineState::UP;
            break;
        case 1:
            humanState.mFacing = MachineState::DOWN;
            break;
        case 2:
            humanState.mFacing = MachineState::LEFT;
            break;
        case 3:
            humanState.mFacing = MachineState::RIGHT;
            break;
    }
    human->BindState(humanState);
    mHumans.push_back(human);
    mHumanStates.push_back(std::make_shared<MachineState>(humanState));
    mGrid[row][col] = 2;
}

std::shared_ptr<Machine<ZombieTraits>> ZomWorld::GetZombie(int index)
{
    return mZombies.at(index);
}

std::shared_ptr<Machine<HumanTraits>> ZomWorld::GetHuman(int index)
{
    return mHumans.at(index);
}

std::shared_ptr<MachineState> ZomWorld::GetZombieState(int index)
{
    return mZombieStates.at(index);
}

std::shared_ptr<MachineState> ZomWorld::GetHumanState(int index)
{
    return mHumanStates.at(index);
}

int ZomWorld::GetNumZombies() const noexcept
{
    return static_cast<int>(mZombies.size());
}

int ZomWorld::GetNumHumans() const noexcept
{
    return static_cast<int>(mHumans.size());
}

int ZomWorld::GetGrid(int row, int col) const noexcept
{
    return mGrid[row][col];
}

void ZomWorld::SetGrid(int row, int col, int value)
{
    mGrid[row][col] = value;
}

void ZomWorld::KillAt(int row, int col)
{
    int y = row * 30;
    int x = col * 30;
    //if zombie occupied
    if(mGrid[row][col] == 1)
    {
        int index = -1;
        //find the zombie
        for(int i = 0; i < mZombieStates.size(); i++)
        {
            if((mZombieStates.at(i)->mX == x)&&(mZombieStates.at(i)->mY == y))
            {
                index = i;
            }
        }
        //kill it
        mZombies.erase(mZombies.begin() + index);
        mZombieStates.erase(mZombieStates.begin() + index);
    }
    //if human occupied
    else if(mGrid[row][col] == 2)
    {
        int index = -1;
        //find the human
        for(int i = 0; i < mHumanStates.size(); i++)
        {
            if((mHumanStates.at(i)->mX == x)&&(mHumanStates.at(i)->mY == y))
            {
                index = i;
            }
        }
        //kill it
        mHumans.erase(mHumans.begin() + index);
        mHumanStates.erase(mHumanStates.begin() + index);
    }
    //set grid to unoccupied
    mGrid[row][col] = 0;
}

void ZomWorld::TurnZombieAt(int row, int col)
{
    int y = row * 30;
    int x = col * 30;
    if(mGrid[row][col] == 2)
    {
        int index = -1;
        for(int i = 0; i < mHumanStates.size(); i++)
        {
            if((mHumanStates.at(i)->mX == x)&&(mHumanStates.at(i)->mY == y))
            {
                index = i;
            }
        }
        Machine<ZombieTraits> newZombie;
        newZombie.LoadMachine(ZomWorld::get().GetZombieFileName());
        MachineState newState;
        newState.mX = x;
        newState.mY = y;
        newZombie.BindState(newState);
        mZombies.push_back(std::make_shared<Machine<ZombieTraits>>(newZombie));
        mZombieStates.push_back(std::make_shared<MachineState>(newState));
        mHumans.erase(mHumans.begin() + index);
        mHumanStates.erase(mHumanStates.begin() + index);
    }
    mGrid[row][col] = 1;
}

void ZomWorld::SetHumanFilename(const std::string& filename)
{
    mHumanFilename = filename;
}

std::string ZomWorld::GetHumanFileName() const noexcept
{
    return mHumanFilename;
}

void ZomWorld::SetZombieFilename(const std::string& filename)
{
    mZombieFilename = filename;
}

std::string ZomWorld::GetZombieFileName() const noexcept
{
    return mZombieFilename;
}

void ZomWorld::ClearAll()
{
    mZombies.clear();
    mZombieStates.clear();
    mHumans.clear();
    mHumanStates.clear();
}

int ZomWorld::GetMonth() const noexcept
{
    return mMonth;
}

void ZomWorld::SetMonth(int month)
{
    mMonth = month;
}

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
    mHumans.clear();
    mZombies.clear();
    mHumanStates.clear();
    mZombieStates.clear();
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
    zombie->BindState(zombieState);
    mZombies.push_back(zombie);
    mZombieStates.push_back(std::make_shared<MachineState>(zombieState));
    mGrid[0][0] = 1;
}

void ZomWorld::AddHuman(std::shared_ptr<Machine<HumanTraits>> human)
{
    MachineState humanState;
    human->BindState(humanState);
    mHumans.push_back(human);
    mHumanStates.push_back(std::make_shared<MachineState>(humanState));
    mGrid[0][0] = 2;
}

std::shared_ptr<Machine<ZombieTraits>> ZomWorld::GetZombie(int index)
{
    return mZombies.at(index);
}

std::shared_ptr<MachineState> ZomWorld::GetZombieState(int index)
{
    return mZombieStates.at(index);
}

int ZomWorld::GetNumZombies() const
{
    return static_cast<int>(mZombies.size());
}

int ZomWorld::GetGrid(int row, int col) const
{
    return mGrid[row][col];
}

void ZomWorld::SetGrid(int row, int col, int value)
{
    mGrid[row][col] = value;
}

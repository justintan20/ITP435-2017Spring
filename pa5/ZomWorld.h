//
//  ZomWorld.hpp
//  zombiewx-mac
//
//  Created by Justin Tan on 3/29/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#pragma once
#include "Singleton.h"
#include <vector>
#include "Machine.h"
#include "Traits.h"

class ZomWorld : public Singleton<ZomWorld>
{
    DECLARE_SINGLETON(ZomWorld) friend class Singleton<ZomWorld>;
public:
    void NewWorld();
    void AddZombie(std::shared_ptr<Machine<ZombieTraits>> zombie);
    void AddHuman(std::shared_ptr<Machine<HumanTraits>> human);
    std::shared_ptr<Machine<ZombieTraits>> GetZombie(int index);
    std::shared_ptr<MachineState> GetZombieState(int index);
    int GetNumZombies() const;
    int GetGrid(int row, int col) const;
    void SetGrid(int row, int col, int value);
private:
    std::vector<std::shared_ptr<Machine<HumanTraits>>> mHumans;
    std::vector<std::shared_ptr<Machine<ZombieTraits>>> mZombies;
    std::vector<std::shared_ptr<MachineState>> mZombieStates;
    std::vector<std::shared_ptr<MachineState>> mHumanStates;
    int mGrid[20][20];
};

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
    //constructor
    void NewWorld();
    //add zombie and humans to world
    void AddZombie(std::shared_ptr<Machine<ZombieTraits>> zombie);
    void AddHuman(std::shared_ptr<Machine<HumanTraits>> human);
    //get zombie and human machine and machine states
    std::shared_ptr<Machine<ZombieTraits>> GetZombie(int index);
    std::shared_ptr<Machine<HumanTraits>> GetHuman(int index);
    std::shared_ptr<MachineState> GetZombieState(int index);
    std::shared_ptr<MachineState> GetHumanState(int index);
    //get number of zombies and humans
    int GetNumZombies() const noexcept;
    int GetNumHumans() const noexcept;
    //get value at location in grid
    int GetGrid(int row, int col) const noexcept;
    void SetGrid(int row, int col, int value);
    //kill individual at grid location
    void KillAt(int row, int col);
    //turn individual at grid location into zombie
    void TurnZombieAt(int row, int col);
    //set and get filenames for zombies and humans
    void SetHumanFilename(const std::string& filename);
    std::string GetHumanFileName() const noexcept;
    void SetZombieFilename(const std::string& filename);
    std::string GetZombieFileName() const noexcept;
    //clear all humans and zombies from world
    void ClearAll();
    //get the current month
    int GetMonth() const noexcept;
    //set the current month
    void SetMonth(int month);
private:
    //vectors of machines and machine states of humans and zombies
    std::vector<std::shared_ptr<Machine<HumanTraits>>> mHumans;
    std::vector<std::shared_ptr<Machine<ZombieTraits>>> mZombies;
    std::vector<std::shared_ptr<MachineState>> mZombieStates;
    std::vector<std::shared_ptr<MachineState>> mHumanStates;
    //grid: 0 = empty 1 = zombie 2 = human
    int mGrid[20][20];
    //filenames for humans and zombies
    std::string mHumanFilename;
    std::string mZombieFilename;
    //current month
    int mMonth;
};

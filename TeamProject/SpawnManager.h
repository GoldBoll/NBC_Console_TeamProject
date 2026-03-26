#pragma once
#include "Map/Map.h"
#include "Monster/Monster.h"
#include "Goblin.h"
#include "Orc.h"
#include "Mimic.h"
#include "Golem.h"
#include <vector>

class SpawnManager
{
private:
    SpawnManager();
    ~SpawnManager();

    std::vector<Monster*> activeMonsters;

public:
    static SpawnManager* GetInstance()
    {
        static SpawnManager instance;
        return &instance;
    }

    void SpawnMonster(int _mapNumber, Monster* _pmonster, int _targetX, int _targetY);
};


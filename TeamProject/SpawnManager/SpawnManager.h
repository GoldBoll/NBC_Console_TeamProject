#pragma once
#include "Map/Map.h"
#include "../Monster/Monster.h"
#include "../Monster/Goblin.h"
#include "../Monster/Orc.h"
#include "../Monster/Mimic.h"
#include "../Monster/Golem.h"
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


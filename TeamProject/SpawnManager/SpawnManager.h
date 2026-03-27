#pragma once
#include "../Map/Map.h"
#include "../Monster/Monster.h"
#include "../Monster/Goblin.h"
#include "../Monster/Orc.h"
#include "../Monster/Mimic.h"
#include "../Monster/Golem.h"
#include "../Player/Player.h"
#include <vector>

class Map;

class SpawnManager
{
private:
    SpawnManager();
    ~SpawnManager();

    SpawnManager(const SpawnManager&) = delete;
    SpawnManager& operator=(const SpawnManager&) = delete;

    std::vector<Monster*> activeMonsters;

public:
    static SpawnManager* GetInstance()
    {
        static SpawnManager instance;
        return &instance;
    }

    //void SpawnMonsterInRoom(Map& _map, Room& room, int count);

};


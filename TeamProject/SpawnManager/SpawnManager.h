#pragma once
#include "../Map/Map.h"
#include "../Monster/Monster.h"
#include "../Player/Player.h"
#include <vector>

class SpawnManager
{
private:
    SpawnManager();


    SpawnManager(const SpawnManager&) = delete;
    SpawnManager& operator=(const SpawnManager&) = delete;

    std::vector<Monster*> activeMonsters;

public:
    static SpawnManager* GetInstance()
    {
        static SpawnManager instance;
        return &instance;
    }
    ~SpawnManager();

    const std::vector<Monster*>& GetActiveMonsters() const;
    void SpawnMonstersInRooms(Map& _map, const std::vector<Room>& _roomList);
    void UpdateCleanup(Map& _map);
    void SpawnPlayerInRooms(Map& _map, const std::vector<Room>& _roomList, Player* player);
    void SpawnObjectInRooms(Map& _map, const std::vector<Room>& _roomList);
};

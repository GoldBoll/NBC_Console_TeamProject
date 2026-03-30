#include "SpawnManager.h"
#include "../Monster/Goblin.h"
#include "../Monster/Orc.h"
#include "../Monster/Mimic.h"
#include "../Monster/Golem.h"

SpawnManager::SpawnManager()
{
}

SpawnManager::~SpawnManager()
{
    for (Monster* m : activeMonsters)
    {
        delete m;
    }
    activeMonsters.clear();
}

const vector<Monster*>& SpawnManager::GetActiveMonsters() const
{
    return activeMonsters;
}

void SpawnManager::SpawnMonstersInRooms(Map& _map, const std::vector<Room>& _roomList)
{
    for (const auto& room : _roomList)
    {
        if (room.type == RoomType::Start || room.type == RoomType::Stair) continue;

        /*if (room.type == RoomType::Boss)
        {
            int rx = room.x + 1 + (rand() % (room.w - 2));
            int ry = room.y + 1 + (rand() % (room.h - 2));
            int spawned = 0;
            while (spawned < 1)
            {
                if (_map.GetTile(rx, ry) == Tile::Floor)
                {
                    Monster* newBossMonster = new Goblin(true);
                    newBossMonster->SetPosition(rx, ry);
                    activeMonsters.push_back(newBossMonster);
                    _map.SetTile(rx, ry, Tile::Boss);
                    spawned++;
                }
            }
            continue;
        }*/

        else if (room.type == RoomType::Treasure || room.type == RoomType::Elite)
        {
            int rx = room.x + 1 + (rand() % (room.w - 2));
            int ry = room.y + 1 + (rand() % (room.h - 2));
            int spawned = 0;
            while (spawned < 1)
            {
                if (_map.GetTile(rx, ry) == Tile::Floor)
                {
                    Monster* newEliteMonster = new Goblin(true);
                    newEliteMonster->SetPosition(rx, ry);
                    activeMonsters.push_back(newEliteMonster);
                    _map.SetTile(rx, ry, Tile::EliteMonster);
                    spawned++;
                }
            }
            continue;
        }
        
        else if (room.type == RoomType::Normal)
        {
            int monsterCount = (rand() % 4) + 1;

            int spawned = 0;
            int attempts = 0;

            while (spawned < monsterCount && attempts < 10)
            {
                attempts++;

                int rx = room.x + 1 + (rand() % (room.w - 2));
                int ry = room.y + 1 + (rand() % (room.h - 2));

                if (_map.GetTile(rx, ry) == Tile::Floor)
                {
                    Monster* newMonster = new Goblin(false);
                    newMonster->SetPosition(rx, ry);
                    activeMonsters.push_back(newMonster);
                    _map.SetTile(rx, ry, Tile::Monster);

                    spawned++;
                }
            }
        }
    }
}

void SpawnManager::UpdateCleanup(Map& _map)
{
    auto it = activeMonsters.begin();
    while (it != activeMonsters.end())
    {
        Monster* m = *it;

        if (m->IsDead())
        {
            _map.SetTile(m->GetX(), m->GetY(), Tile::Floor);

            delete m;

            it = activeMonsters.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

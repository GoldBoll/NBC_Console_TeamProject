#include "SpawnManager.h"
#include "../Monster/Goblin.h"
#include "../Monster/Orc.h"
#include "../Monster/Mimic.h"
#include "../Monster/Golem.h"
#include <cstdlib>

SpawnManager::SpawnManager()
{
}

SpawnManager::~SpawnManager()
{
    for (Monster* m : activeMonsters)
    {
        if (m) delete m;
    }
    activeMonsters.clear();
}

const std::vector<Monster*>& SpawnManager::GetActiveMonsters() const
{
    return activeMonsters;
}

void SpawnManager::SpawnMonstersInRooms(Map& _map, const std::vector<Room>& _rooms, int playerX, int playerY)
{
    activeMonsters.clear();

    for (const auto& room : _rooms)
    {
        // 시작 방(Type::Start)에는 일반 몹 스폰 안함
        if (room.type == RoomType::Start) continue;

        // 엘리트 혹은 보물방일 때 (엘리트 몹 1마리)
        if (room.type == RoomType::Elite || room.type == RoomType::Treasure)
        {
            int rx = room.x + 1 + (rand() % (max(1, room.w - 2)));
            int ry = room.y + 1 + (rand() % (max(1, room.h - 2)));

            // 플레이어와 겹치지 않을 때까지 좌표 재선정 (최대 10번 시도)
            for(int attempt=0; attempt<10; ++attempt)
            {
                if (rx != playerX || ry != playerY) break;
                rx = room.x + 1 + (rand() % (max(1, room.w - 2)));
                ry = room.y + 1 + (rand() % (max(1, room.h - 2)));
            }

            if (_map.GetTile(rx, ry) == Tile::Floor)
            {
                Monster* m = new Goblin(true);
                m->SetPosition(rx, ry);
                activeMonsters.push_back(m);
                _map.SetTile(rx, ry, Tile::EliteMonster);
            }
        }
        // 일반 방일 때
        else if (room.type == RoomType::Normal)
        {
            int monsterCount = (rand() % 3) + 1;
            for (int i = 0; i < monsterCount; ++i)
            {
                int rx = room.x + 1 + (rand() % (max(1, room.w - 2)));
                int ry = room.y + 1 + (rand() % (max(1, room.h - 2)));

                if (_map.GetTile(rx, ry) == Tile::Floor && (rx != playerX || ry != playerY))
                {
                    Monster* m = (rand() % 2 == 0) ? (Monster*)new Goblin(false) : (Monster*)new Orc(false);
                    m->SetPosition(rx, ry);
                    activeMonsters.push_back(m);
                    _map.SetTile(rx, ry, Tile::Monster);
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
        if (m && m->IsDead())
        {
            if (_map.InBounds(m->GetX(), m->GetY()))
            {
                _map.SetTile(m->GetX(), m->GetY(), Tile::Floor);
            }
            delete m;
            it = activeMonsters.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void SpawnManager::SpawnPlayerInRooms(Map& _map, const std::vector<Room>& _roomList, Player* player)
{
    for (const auto& room : _roomList)
    {
        if (room.type == RoomType::Start)
        {
            player->SetX(room.CenterX());
            player->SetY(room.CenterY());
        }
    }
}

void SpawnManager::SpawnObjectInRooms(Map& _map, const std::vector<Room>& _roomList)
{
    for (const auto& room : _roomList)
    {
        int rx = room.CenterX();
        int ry = room.CenterY();
        if (room.type == RoomType::Treasure)
        {
            if (_map.GetTile(rx, ry) == Tile::Floor)
            {
                _map.SetTile(rx, ry, Tile::Chest);
            }
        }
        else if (room.type == RoomType::Stair)
        {
            if (_map.GetTile(rx, ry) == Tile::Floor)
            {
                _map.SetTile(rx, ry, Tile::Stair);
            }
        }
    }
}

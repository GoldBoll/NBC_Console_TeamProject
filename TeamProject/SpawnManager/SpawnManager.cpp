#include "SpawnManager.h"
#include "../Monster/Goblin.h"
#include "../Monster/Orc.h"
#include "../Monster/Mimic.h"
#include "../Monster/Golem.h"
#include "../Monster/Boss.h"
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

void SpawnManager::SpawnMonstersInRooms(Map& _map, const std::vector<Room>& _rooms, int _stage)
{
    activeMonsters.clear();

    switch (_stage)
    {
    case 1:
        Stage1(_map, _rooms);
        break;
    case 2:
        Stage2(_map, _rooms);
        break;
    case 3:
        Stage3(_map, _rooms);
        break;
    case 4:
        Stage4(_map, _rooms);
        break;
    default:
        break;
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

Monster* SpawnManager::GetMonsterAt(int targetX, int targetY)
{
    for (auto m : activeMonsters)
    {
        if (m->GetX() == targetX && m->GetY() == targetY)
        {
            return m;
        }
    }
    return nullptr;
}

void SpawnManager::Stage1(Map& _map, const std::vector<Room>& _rooms)
{
    activeMonsters.clear();

    for (const auto& room : _rooms)
    {
        // 시작 방(Type::Start)에는 일반 몹 스폰 안함
        if (room.type == RoomType::Start) continue;

        // 엘리트 혹은 보물방일 때 (엘리트 몹 1마리)
        if (room.type == RoomType::Elite || room.type == RoomType::Treasure)
        {
            for (int spawncount = 0; spawncount < 1;)
            {
                int rx = room.x + 1 + (rand() % (max(1, room.w - 2)));
                int ry = room.y + 1 + (rand() % (max(1, room.h - 2)));
                if (_map.GetTile(rx, ry) == Tile::Floor)
                {
                    Monster* m = nullptr;

                    if (rand() % 2 == 0)
                    {
                        m = new Goblin(true);
                    }
                    else
                    {
                        m = new Orc(true);
                    }
                    m->SetPosition(rx, ry);
                    activeMonsters.push_back(m);
                    _map.SetTile(rx, ry, Tile::EliteMonster);
                    spawncount++;
                }
            }
        }
        // 일반 방일 때
        else if (room.type == RoomType::Normal)
        {
            int spawnCount = 0;
            int monsterCount = (rand() % 3) + 1;
            for (int i = 0; i < monsterCount; ++i)
            {
                if (spawnCount < 12)
                {
                    int rx = room.x + 1 + (rand() % (max(1, room.w - 2)));
                    int ry = room.y + 1 + (rand() % (max(1, room.h - 2)));

                    if (_map.GetTile(rx, ry) == Tile::Floor)
                    {
                        Monster* m = nullptr;

                        if (rand() % 2 == 0)
                        {
                            m = new Goblin(false);
                        }
                        else
                        {
                            m = new Orc(false);
                        }

                        m->SetPosition(rx, ry);
                        activeMonsters.push_back(m);
                        _map.SetTile(rx, ry, m->GetDisplayTile());
                        spawnCount++;
                    }
                }
            }
        }
    }
}

void SpawnManager::Stage2(Map& _map, const std::vector<Room>& _rooms)
{
    activeMonsters.clear();

    for (const auto& room : _rooms)
    {
        // 시작 방(Type::Start)에는 일반 몹 스폰 안함
        if (room.type == RoomType::Start) continue;

        // 엘리트 혹은 보물방일 때 (엘리트 몹 1마리)
        if (room.type == RoomType::Elite || room.type == RoomType::Treasure)
        {
            for (int spawncount = 0; spawncount < 1;)
            {
                int rx = room.x + 1 + (rand() % (max(1, room.w - 2)));
                int ry = room.y + 1 + (rand() % (max(1, room.h - 2)));
                if (_map.GetTile(rx, ry) == Tile::Floor)
                {
                    Monster* m = nullptr;
                    int num = rand() % 3;

                    if (num == 0)
                    {
                        m = new Goblin(true);
                    }
                    else if(num == 1)
                    {
                        m = new Orc(true);
                    }
                    else
                    {
                        m = new Mimic(true);
                    }
                    m->SetPosition(rx, ry);
                    activeMonsters.push_back(m);
                    _map.SetTile(rx, ry, Tile::EliteMonster);
                    spawncount++;
                }
            }
        }
        // 일반 방일 때
        else if (room.type == RoomType::Normal)
        {
            int spawnCount = 0;
            int monsterCount = (rand() % 3) + 1;
            for (int i = 0; i < monsterCount; ++i)
            {
                if (spawnCount < 20)
                {
                    int rx = room.x + 1 + (rand() % (max(1, room.w - 2)));
                    int ry = room.y + 1 + (rand() % (max(1, room.h - 2)));

                    if (_map.GetTile(rx, ry) == Tile::Floor)
                    {
                        Monster* m = nullptr;
                        int num = rand() % 3;
                        if (num == 0)
                        {
                            m = new Goblin(false);
                        }
                        else if (num ==1)
                        {
                            m = new Orc(false);
                        }
                        else
                        {
                            m = new Mimic(false);
                        }

                        m->SetPosition(rx, ry);
                        activeMonsters.push_back(m);
                        _map.SetTile(rx, ry, m->GetDisplayTile());
                        spawnCount++;
                    }
                }
            }
        }
    }
}

void SpawnManager::Stage3(Map& _map, const std::vector<Room>& _rooms)
{
    activeMonsters.clear();

    for (const auto& room : _rooms)
    {
        // 시작 방(Type::Start)에는 일반 몹 스폰 안함
        if (room.type == RoomType::Start) continue;

        // 엘리트 혹은 보물방일 때 (엘리트 몹 1마리)
        if (room.type == RoomType::Elite || room.type == RoomType::Treasure)
        {
            for (int spawncount = 0; spawncount < 1;)
            {
                int rx = room.x + 1 + (rand() % (max(1, room.w - 2)));
                int ry = room.y + 1 + (rand() % (max(1, room.h - 2)));
                if (_map.GetTile(rx, ry) == Tile::Floor)
                {
                    Monster* m = nullptr;
                    int num = rand() % 4;

                    if (num == 0)
                    {
                        m = new Goblin(true);
                    }
                    else if (num == 1)
                    {
                        m = new Orc(true);
                    }
                    else if (num == 2)
                    {
                        m = new Mimic(true);
                    }
                    else
                    {
                        m = new Golem(true);
                    }
                    m->SetPosition(rx, ry);
                    activeMonsters.push_back(m);
                    _map.SetTile(rx, ry, Tile::EliteMonster);
                    spawncount++;
                }
            }
        }
        // 일반 방일 때
        else if (room.type == RoomType::Normal)
        {
            int spawnCount = 0;
            int monsterCount = (rand() % 3) + 1;
            for (int i = 0; i < monsterCount; ++i)
            {
                if (spawnCount < 22)
                {
                    int rx = room.x + 1 + (rand() % (max(1, room.w - 2)));
                    int ry = room.y + 1 + (rand() % (max(1, room.h - 2)));

                    if (_map.GetTile(rx, ry) == Tile::Floor)
                    {
                        Monster* m = nullptr;
                        int num = rand() % 3;
                        if (num == 0)
                        {
                            m = new Goblin(false);
                        }
                        else if (num == 1)
                        {
                            m = new Orc(false);
                        }
                        else if (num == 2)
                        {
                            m = new Mimic(false);
                        }
                        else
                        {
                            m = new Golem(false);
                        }
                        m->SetPosition(rx, ry);
                        activeMonsters.push_back(m);
                        _map.SetTile(rx, ry, m->GetDisplayTile());
                        spawnCount++;
                    }
                }
            }
        }
    }
}

void SpawnManager::Stage4(Map& _map, const std::vector<Room>& _rooms)
{
    activeMonsters.clear();

    for (const auto& room : _rooms)
    {
        // 시작 방(Type::Start)에는 일반 몹 스폰 안함
        if (room.type == RoomType::Boss)
        {
            int rx = room.CenterX();
            int ry = room.CenterY() + -10;

            Monster* boss = new Boss();
            boss->SetPosition(rx, ry);
            activeMonsters.push_back(boss);
            _map.SetTile(rx, ry, boss->GetDisplayTile());
        }
    }
}

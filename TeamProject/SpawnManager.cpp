#include "SpawnManager.h"

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
void SpawnManager::SpawnMonster(int _mapNumber, Monster* _pmonster, int _targetX, int _targetY)
{
    
}

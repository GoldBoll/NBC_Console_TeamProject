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

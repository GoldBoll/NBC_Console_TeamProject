#pragma once
#include <vector>
#include "../Map/Map.h"
#include "../Player/Warrior.h"
#include "../Player/Player.h"
#include "../Monster/Monster.h"
#include "../GameManager/InputManager.h"
#include "../BspManager/BspManager.h"

class GameManager
{
public:
    static GameManager& GetInstance();

    void Run();

    GameManager(const GameManager&)            = delete;
    GameManager& operator=(const GameManager&) = delete;

private:
    GameManager();

    void Init();
    void HandleAction(GameAction action);

    Map                  map;
    Player*              player;
    std::vector<Monster> monsters;
    bool                 running     = false;
    bool                 needsRedraw = false;
};

#pragma once
#include <vector>
#include "../Map/Map.h"
#include "../Player/Warrior.h"
#include "../Player/Player.h"
#include "../Monster/Monster.h"
#include "../GameManager/InputManager.h"
#include "../Battle/Battle.h"

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
    void UpdateMonster();
    void ProcessBattle();

    Map                  map;
    Player*              player;
    std::vector<Monster*> monsters;
    bool                 running     = false;
    bool                 needsRedraw = false;
    bool                 inputbutton = false;
    bool                 isBattleMode = false;
    bool                 isNextMoveDash = false;
    Monster*             battleTarget = nullptr;
    Battle               battle;
};

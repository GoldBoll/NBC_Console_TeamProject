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
    void Init(int stage);

    GameManager(const GameManager&)            = delete;
    GameManager& operator=(const GameManager&) = delete;

    int GetCurrentStageIndex() const { return curStage; }
    void SetCurrentStageIndex(int stage) { curStage = stage; }

private:
    GameManager();

    void HandleAction(GameAction action);
    void UpdateMonster();
    void ProcessBattle();

    void OpenInventory();
    void CloseInventory();
    void HandleInventoryAction(GameAction action);



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
    bool                inventoryOpen    = false;
    int                 invSelectedIdx   = 0;
    int                 invScrollOffset  = 0;
    int                 curStage         = 1;
};

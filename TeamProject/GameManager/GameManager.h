#pragma once
#include <vector>
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../Player/Warrior.h"
#include "../Monster/Monster.h"
#include "../GameManager/InputManager.h"
#include "../Battle/Battle.h"
#include "../Item/ItemManager.h"

class GameManager
{
public:
    static GameManager& GetInstance();

    void Run();
    void Init(int stage);

    GameManager(const GameManager&)            = delete;
    GameManager& operator=(const GameManager&) = delete;

    int GetCurrentStageIndex() const { return curStage; }
    int GetCurrentStageIndex() const { return curStage; }
    void SetCurrentStageIndex(int stage) { curStage = stage; }
    Map& GetMap() { return map; }

private:
    GameManager();

    void HandleAction(GameAction action);
    void UpdateMonster();
    void ProcessBattle();
    void SceneChange(int stage);

    Map                  map;
    Player*              player;
    std::vector<Monster*> monsters;
    bool                 running        = false;
    bool                 needsRedraw    = false;
    bool                 inputbutton    = false;
    bool                 isBattleMode   = false;
    bool                 isNextMoveDash = false;
    Monster*             battleTarget   = nullptr;
    Battle               battle;
    ItemManager&         itemMgr        = ItemManager::GetInstance();
    int                  curStage       = 1;
};

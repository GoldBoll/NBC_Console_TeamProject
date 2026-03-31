#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "../GameManager/InputManager.h"
#include "../Item/Inventory.h"
#include "../Map/Map.h"

struct Map;

class Monster;
enum class Tile;

class Player
{
protected:
    // 위치
    int x = 0, y = 0;

    int combatRange = 2;
    int chaseRange = 4;

    // status
    std::string jobName;
    int hp;
    int maxHp;
    int atk;
    int def;
    int dex;

    int level;
    int exp;
    int expToNext;
    //int gold      = 0;

    // Level up bonus
    int maxHpBonus;
    int atkBonus;
    int defBonus;
    int dexBonus;
    int hpBonus;

    Inventory            inventory;
    // vector<StatusEffect> statusEffects;

    int dashGauge = 120;
    const int maxDashGauge = 120;
    int equippedWeaponHit = 90;

public:
    Player(std::string _jobName);
    ~Player();

    // 나중에 2칸씩 움직이는 캐릭터도 있을 수 있으니 일단 가상함수로 구현
    virtual void TryMove(int _dx, int _dy, Map& _map) = 0;

    //virtual void attack() = 0;
    //virtual void attack(Monster* monster) = 0;

    // Getter
    Inventory&       GetInventory()       { return inventory; }
    const Inventory& GetInventory() const { return inventory; }

    std::string GetJobName() const { return jobName; }
    int GetLevel()     const { return level; }
    int GetHP()        const { return hp; }
    int GetMaxHP()     const { return maxHp; }
    int GetExp()       const { return exp; }
    int GetExpToNext() const { return expToNext; }
    int GetAtk()       const { return atk; }
    int GetDef()       const { return def; }
    int GetDex()       const { return dex; }
    int GetX()         const { return x; }
    int GetY()         const { return y; }

    // Setter
    void SetLevel(int _level);
    void SetHp(int _hp);
    void SetAtk(int _atk);
    void SetDef(int _def);
    void SetDex(int _dex);
    void SetX(int _x) { x = _x; }
    void SetY(int _y) { y = _y; }

    // player info
    bool IsAlive() const { return hp > 0; }
    //void PrintStatus() const;

    void GainExp(int _exp);
    void LevelUp();
    void TakeDamage(int _damage, int attackerDex = 0);

    void UpdateDash();
    bool CanDash() const { return dashGauge >= maxDashGauge; }
    void UseDash() { dashGauge = 0; }

    int GetDashGauge() const { return dashGauge; }

    bool Attack(Monster* target);
    void DetectMonsters(std::vector<Monster*>& _monsters);
    bool Move(GameAction action, Map& _map);
    bool TileCheck(int _dx, int _dy, Map& _map);
    void OnCollision(Tile targetTile);
};

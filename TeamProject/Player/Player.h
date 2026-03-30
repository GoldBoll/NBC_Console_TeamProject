#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "../GameManager/InputManager.h"

struct Map;
struct Monster;

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
    int dex;

    int level;
    int exp;
    int expToNext;
    //int gold      = 0;

    // Level up bonus
    int maxHpBonus;
    int atkBonus;
    int dexBonus;
    int hpBonus;

    // Inventory            inventory;
    // vector<StatusEffect> statusEffects;

public:
    Player(std::string _jobName);
    ~Player();

    // 나중에 2칸씩 움직이는 캐릭터도 있을 수 있으니 일단 가상함수로 구현
    virtual void TryMove(int _dx, int _dy, Map& _map) = 0;

    //virtual void attack() = 0;
    //virtual void attack(Monster* monster) = 0;

    // Getter
    std::string GetJobName() const { return jobName; }
    int GetLevel()     const { return level; }
    int GetHP()        const { return hp; }
    int GetMaxHP()     const { return maxHp; }
    int GetExp()       const { return exp; }
    int GetExpToNext() const { return expToNext; }
    int GetAtk()       const { return atk; }
    int GetDex()       const { return dex; }
    int GetX()         const { return x; }
    int GetY()         const { return y; }

    // Setter
    void SetLevel(int _level);
    void SetHp(int _hp);
    void SetAtk(int _atk);
    void SetDex(int _dex);
    void SetX(int _x) { x = _x; }
    void SetY(int _y) { y = _y; }

    // player info
    bool IsAlive() const { return hp > 0; }
    //void PrintStatus() const;

    void GainExp(int _exp);
    void LevelUp();
    void TakeDamage(int _damage);

    void DetectMonsters(std::vector<Monster*>& _monsters);
    void Move(GameAction action, Map& _map);
    bool TileCheck(int _x, int _y, Map& _map);
};

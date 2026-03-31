#pragma once
#include <string>
#include "../Map/Map.h"

class Player;

enum class MonsterState {
    IDLE,
    CHASE,
    COMBAT
};

class Monster
{
protected:
    std::string name;
    int hp;
    int atk;
    int def;
    int dex;
    int exp;
    int x;
    int y;
    bool isElite;

    MonsterState state = MonsterState::IDLE;

    int equippedWeaponHit = 90;

public:
    Monster(std::string _name, int _hp, int _atk, int _def, int _dex, int _exp, bool _elite);
    virtual ~Monster() {};

    // Getters
    std::string GetName()       const { return name; }
    int GetHP()                 const { return hp; }
    int GetAtk()                const { return atk; }
    int GetDef()                const { return def; }
    int GetDex()                const { return dex; }
    int GetExp()                const { return exp; }
    int GetX()                  const { return x; }
    int GetY()                  const { return y; }
    MonsterState GetState()     const { return state; }
    bool IsElite()              const { return isElite; }
    bool IsDead()               const { return hp <= 0; }

    // Setters
    void SetName(std::string _name) { name = _name; }
    void SetHP(int _hp)             { hp = _hp; }
    void SetAtk(int _atk)           { atk = _atk; }
    void SetDef(int _def)           { def = _def; }
    void SetDex(int _dex)           { dex = _dex; }
    void SetPosition(int _newX, int _newY) { x = _newX; y = _newY; }
    void SetState(MonsterState _state) { state = _state; }

    // Actions
    void TakeDamage(int _damage, int attackerDex = 0);
    void Move(int _x, int _y, Map& _map);
    void UpdateAI(Map& _map);

    void Update(int playerX, int playerY, Map& _map);
    void MoveTowards(int targetX, int targetY, Map& _map);

    bool Attack(int playerX, int playerY);
    bool Attack(Player* player);
};

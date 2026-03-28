#pragma once
#include <string>
#include "../Map/Map.h"

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
    int dex;
    int exp;
    int x;
    int y;

    MonsterState state = MonsterState::IDLE;

public:
    Monster(std::string _name, int _hp, int _power, int _speed, int _exp);
    virtual ~Monster() {};

    std::string GetName();
    int GetHP();
    int GetAtk();
    int GetDex();
    int GetX();
    int GetY();
    MonsterState GetState() const { return state; }

    void SetName(std::string _name);
    void SetHP(int _hp);
    void SetAtk(int _atk);
    void SetDex(int _dex);
    void SetPosition(int _newX, int _newY);
    void SetState(MonsterState _state) { state = _state; }

    void TakeDamage(int _damage);
    bool IsDead();

    void Move(int _x, int _y, Map& _map);
    void UpdateAI(Map& _map);

    virtual void Update(int playerX, int playerY);
    void MoveTowards(int targetX, int targetY);
};

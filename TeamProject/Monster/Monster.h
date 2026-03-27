#pragma once
#include <string>
#include "../Map/Map.h"

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

public:
    Monster(std::string _name, int _hp, int _power, int _speed, int _exp);
    virtual ~Monster() {};

    std::string GetName();
    int GetHP();
    int GetAtk();
    int GetDex();
    int GetX();
    int GetY();

    void SetName(std::string _name);
    void SetHP(int _hp);
    void SetAtk(int _atk);
    void SetDex(int _dex);
    void SetPosition(int _newX, int _newY);

    void TakeDamage(int _damage);
    bool IsDead();

    void Move(int _x, int _y, Map& _map);
    void UpdateAI(Map& _map);
};

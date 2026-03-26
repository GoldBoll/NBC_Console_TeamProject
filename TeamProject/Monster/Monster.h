#pragma once
#include <string>

class Monster
{
protected:
    std::string _name;
    int _hp;
    int _power;
    int _speed;
    int _exp;
    int _x;
    int _y;

public:
    Monster(std::string name, int hp, int power, int speed, int exp);

    std::string GetName();
    int GetHP();
    int GetPower();
    int GetSpeed();
    int GetX();
    int GetY();

    void SetName(std::string name);
    void SetHP(int hp);
    void SetPower(int power);
    void SetSpeed(int speed);
    void SetPosition(int newX, int newY);

    void TakeDamage(int damage);
};

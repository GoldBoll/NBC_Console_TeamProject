#pragma once
#include <string>

class Monster
{
protected:
    std::string M_name;
    int M_hp;
    int M_power;
    int M_speed;

public:
    Monster(std::string name, int hp, int power, int speed);

    std::string GetName();
    int GetHP();
    int GetPower();
    int GetSpeed();

    void SetName(std::string name);
    void SetHP(int hp);
    void SetPower(int power);
    void SetSpeed(int speed);

    void TakeDamage(int damage);
};

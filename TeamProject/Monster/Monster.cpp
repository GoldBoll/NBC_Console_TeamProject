#include "Monster.h"
#include <iostream>

Monster::Monster(std::string name, int hp, int power, int speed)
    :M_name(name), M_hp(hp), M_power(power), M_speed(speed)
{
}

std::string Monster::GetName()
{
    return M_name;
}

int Monster::GetHP()
{
    return M_hp;
}

int Monster::GetPower()
{
    return M_power;
}

int Monster::GetSpeed()
{
    return M_speed;
}

void Monster::SetName(std::string name)
{
    M_name = name;
}

void Monster::SetHP(int hp)
{
    M_hp = hp;
}

void Monster::SetPower(int power)
{
    M_power = power;
}

void Monster::SetSpeed(int speed)
{
    M_speed = speed;
}

void Monster::TakeDamage(int damage)
{
    M_hp -= damage;
}

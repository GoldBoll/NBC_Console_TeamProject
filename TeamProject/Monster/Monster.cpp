#include "Monster.h"
#include <iostream>

Monster::Monster(std::string name, int hp, int power, int speed, int exp)
    :_name(name), _hp(hp), _power(power), _speed(speed), _exp(exp)
{
}

std::string Monster::GetName()
{
    return _name;
}

int Monster::GetHP()
{
    return _hp;
}

int Monster::GetPower()
{
    return _power;
}

int Monster::GetSpeed()
{
    return _speed;
}

int Monster::GetX()
{
    return _x;
}

int Monster::GetY()
{
    return _y;
}

void Monster::SetName(std::string name)
{
    _name = name;
}

void Monster::SetHP(int hp)
{
    _hp = hp;
}

void Monster::SetPower(int power)
{
    _power = power;
}

void Monster::SetSpeed(int speed)
{
    _speed = speed;
}

void Monster::SetPosition(int newX, int newY)
{
    _x = newX;
    _y = newY;
}

void Monster::TakeDamage(int damage)
{
    _hp -= damage;
}

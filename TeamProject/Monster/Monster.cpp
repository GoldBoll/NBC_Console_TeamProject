#include "Monster.h"

Monster::Monster(std::string _name, int _hp, int _atk, int _dex, int _exp)
    :name(_name), hp(_hp), atk(_atk), dex(_dex), exp(_exp) ,x(0), y(0)
{
}

std::string Monster::GetName()
{
    return name;
}

int Monster::GetHP()
{
    return hp;
}

int Monster::GetAtk()
{
    return atk;
}

int Monster::GetDex()
{
    return dex;
}

int Monster::GetX()
{
    return x;
}

int Monster::GetY()
{
    return y;
}

void Monster::SetName(std::string _name)
{
    name = _name;
}

void Monster::SetHP(int _hp)
{
    hp = _hp;
}

void Monster::SetAtk(int _atk)
{
    atk = _atk;
}

void Monster::SetDex(int _dex)
{
    dex = _dex;
}

void Monster::SetPosition(int _newX, int _newY)
{
    x = _newX;
    y = _newY;
}

void Monster::TakeDamage(int _damage)
{
    hp -= _damage;
}

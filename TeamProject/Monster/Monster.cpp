#include "Monster.h"
#include <cstdlib>

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

void Monster::Update(int playerX, int playerY)
{
    switch (state)
    {
        case MonsterState::IDLE:
        break;
        case MonsterState::CHASE:
            // 턴제 게임의 재미를 위해 70% 확률로만 이동합니다.
            // (나머지 30% 확률로 플레이어에게 도망갈 기회를 줍니다)
            if (rand() % 10 < 7) 
            {
                MoveTowards(playerX, playerY);
            }
        break;
        case MonsterState::COMBAT:
        break;
        default:
        break;
    }
}

void Monster::MoveTowards(int targetX, int targetY)
{
    int dx = targetX - x;
    int dy = targetY - y;
    
    if (abs(dx) > abs(dy))
    {
        if (dx > 0) x++; else x--;
    } 
    else if (dy != 0)
    {
        if (dy > 0) y++; else y--;
    }
}
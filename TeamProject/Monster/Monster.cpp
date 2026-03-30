#include "Monster.h"
#include <cstdlib>

Monster::Monster(std::string _name, int _hp, int _atk, int _dex, int _exp, bool _elite)
    :name(_name), hp(_hp), atk(_atk), dex(_dex), exp(_exp), x(0), y(0), isElite(false)
{
    if (isElite)
    {
        name = "[Elite]" + _name;
        hp = _hp * 2;
        atk = _atk * 2;
        dex = _dex * 2;
        exp = _exp * 2;
    }
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

bool Monster::IsDead()
{
    return hp <= 0;
}

void Monster::Move(int _x, int _y, Map& _map)
{
    int nextX = x + _x;
    int nextY = y + _y;

    if (_map.InBounds(nextX, nextY) && _map.GetTile(nextX, nextY) == Tile::Floor)
    {
        if (_map.GetTile(x, y) == Tile::EliteMonster)
        {
            _map.SetTile(x, y, Tile::Floor);

            x = nextX;
            y = nextY;

            _map.SetTile(x, y, Tile::EliteMonster);
            return;
        }

        _map.SetTile(x, y, Tile::Floor);

        x = nextX;
        y = nextY;

        _map.SetTile(x, y, Tile::Monster);
    }
}

void Monster::UpdateAI(Map& _map)
{
    // 너무 자주 움직이지 않게 확률 부여 (30% 확률로 이동)
    if (rand() % 100 > 30) return;

    int direction = rand() % 4;
    int dx = 0, dy = 0;

    switch (direction)
    {
    case 0:
        dy = -1; // 위
        break;

    case 1:
        dy = 1;  // 아래
        break;

    case 2:
        dx = -1; // 왼쪽
        break;

    case 3:
        dx = 1;  // 오른쪽
        break;

    default:
        break;
    }

    int nextX = x + dx;
    int nextY = y + dy;

    if (_map.GetTile(nextX, nextY)== Tile::Floor)
    {
        this->Move(dx, dy, _map);
    }
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

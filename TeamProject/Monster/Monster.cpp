#include "Monster.h"
#include "../Render/Render.h"
#include <cstdlib>

Monster::Monster(std::string _name, int _hp, int _atk, int _def, int _dex, int _exp, bool _elite)
    :name(_name), hp(_hp), atk(_atk), def(_def), dex(_dex), exp(_exp), x(0), y(0), isElite(_elite)
{
    if (isElite)
    {
        name = "[Elite]" + _name;
    }
}

void Monster::TakeDamage(int _damage)
{
    // 방어력 계산 적용
    // 최종 데미지 = (공격력 × 무기배율) × (1.0 - (방어력 × 0.03))
    int damageAfterDef = static_cast<int>(_damage * (1.0 - (def * 0.03)));

    hp -= damageAfterDef;
}

void Monster::Move(int _dx, int _dy, Map& _map)
{
    int nextX = x + _dx;
    int nextY = y + _dy;

    // 현재 위치와 다음 위치가 모두 유효해야 동작
    if (_map.InBounds(x, y) && _map.InBounds(nextX, nextY) && _map.GetTile(nextX, nextY) == Tile::Floor)
    {
        Tile currentTile = _map.GetTile(x, y);

        // 이전 위치 타일 정리
        _map.SetTile(x, y, Tile::Floor);

        x = nextX;
        y = nextY;

        // 자신의 상태(Elite 여부)에 맞는 타일 배치
        if (isElite)
            _map.SetTile(x, y, Tile::EliteMonster);
        else
            _map.SetTile(x, y, Tile::Monster);
    }
}

void Monster::UpdateAI(Map& _map)
{
    // 추적 상태거나 전투중일 때는 랜덤 이동을 하지 않는다.
    if (state == MonsterState::CHASE || state == MonsterState::COMBAT) return;

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

void Monster::Update(int playerX, int playerY, Map& _map)
{
    switch (state)
    {
        case MonsterState::IDLE:
        break;
        case MonsterState::CHASE:
            MoveTowards(playerX, playerY, _map);
        break;
        case MonsterState::COMBAT:
        break;
        default:
        break;
    }
}

void Monster::MoveTowards(int targetX, int targetY, Map& _map)
{
    int dx = 0;
    int dy = 0;

    int dist_x = targetX - x;
    int dist_y = targetY - y;

    if (abs(dist_x) > abs(dist_y))
    {
        dx = (dist_x > 0) ? 1 : -1;
    }
    else if (dist_y != 0)
    {
        dy = (dist_y > 0) ? 1 : -1;
    }

    Move(dx, dy, _map);
}

bool Monster::Attack(Player* player)
{
    if (!player) return false;

    int monsterDex = this->dex;
    int playerDex = player->GetDex();

    // 명중률 계산: 무기 명중률 + (공격자 DEX - 방어자 DEX) × 2
    int finalHitRate = equippedWeaponHit + (monsterDex - playerDex) * 2;

    if (finalHitRate < 5) finalHitRate = 5;
    else if (finalHitRate > 95) finalHitRate = 95;

    int roll = std::rand() % 100;

    if (roll < finalHitRate)
    {
        int damage = this->atk;
        player->TakeDamage(damage, this->dex);
        Render::GetInstance().AddLog(this->GetName() + "의 공격이 명중했습니다!", CLR_RED);
        return true;
    }
    else
    {
        Render::GetInstance().AddLog(this->GetName() + "의 공격이 빗나갔습니다!", CLR_DARK_GRAY);
        return false;
    }
}

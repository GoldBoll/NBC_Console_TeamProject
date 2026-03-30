#include "Player.h"
#include "../Monster/Monster.h"
#include "../Render/Render.h"
#include <algorithm>
#include <cstdlib>

Player::Player(std::string _jobName)
{
    jobName = _jobName;
    hp = 30;
    maxHp = 30;
    atk = 4;
    def = 0;
    dex = 1;

    level = 1;
    exp = 0;
    expToNext = 20;

    maxHpBonus = 3;
    atkBonus = 1;
    defBonus = 1;
    dexBonus = 1;
    hpBonus = 5;
}

// Setter
void Player::SetLevel(int _level) { level = _level; }
void Player::SetHp(int _hp) { hp = _hp; }
void Player::SetAtk(int _atk) { atk = _atk; }
void Player::SetDef(int _def) { def = _def; }
void Player::SetDex(int _dex) { dex = _dex; }

//void Player::PrintStatus() const {
//    cout << "------------------------------------" << endl;
//    cout << "* 현재 능력치" << endl;
//    cout << "Lv. " << level << endl;
//    cout << "HP: " << hp << endl;
//    cout << "공격력: " << atk << endl;
//    cout << "민첩: " << dex << endl;
//    cout << "경험치: " << exp << endl;
//    cout << "다음 레벨까지의 남은 경험치: " << expToNext - exp << endl;
//    cout << "------------------------------------" << endl;
//}

void Player::GainExp(int _exp)
{
    exp += _exp;

    while (exp >= expToNext)
    {
        exp -= expToNext;
        LevelUp();
    }

}

void Player::LevelUp()
{
    // 선택지 4개 주고 스탯 강화
    int input = 0;

    switch (input)
    {
    case 1:
        maxHp += maxHpBonus;
        break;
    case 2:
        atk += atkBonus;
        break;
    case 3:
        dex += dexBonus;
        break;
    case 4:
        hp += hpBonus;
        if (hp >= maxHp) hp = maxHp;
        break;
    default:
        break;
    }
}

void Player::TakeDamage(int _damage, int attackerDex)
{
    // 명중률 계산: 무기 명중률 + (공격자 DEX - 방어자 DEX) × 2
    int finalHitRate = 90 + (attackerDex - dex) * 2;
    if (finalHitRate < 5) finalHitRate = 5;
    else if (finalHitRate > 95) finalHitRate = 95;

    int roll = std::rand() % 100;

    if (roll < finalHitRate)
    {
        // 명중 시 방어력 계산 적용
        // 최종 데미지 = (공격력 × 무기배율) × (1.0 - (방어력 × 0.03))
        int damageAfterDef = static_cast<int>(_damage * (1.0 - (def * 0.03)));

        hp -= damageAfterDef;
        Render::GetInstance().AddLog("당신은 " + std::to_string(damageAfterDef) + "의 데미지를 입었습니다!", CLR_RED);
    }
    else
    {
        Render::GetInstance().AddLog("공격이 빗나갔습니다!", CLR_WHITE);
    }
}

Player::~Player() { }

void Player::UpdateDash()
{
    int dexBonus = std::min(dex, 10);

    if (dashGauge < maxDashGauge)
    {
        dashGauge += 10 + (dexBonus * 2);

        if (dashGauge > maxDashGauge) dashGauge = maxDashGauge;
    }
}

bool Player::Attack(Monster* target)
{
    if (!target) return false;

    int playerDex = this->dex;
    int monsterDex = target->GetDex();

    // 명중률 계산: 무기 명중률 + (공격자 DEX - 방어자 DEX) × 2
    int finalHitRate = equippedWeaponHit + (playerDex - monsterDex) * 2;

    if (finalHitRate < 5) finalHitRate = 5;
    else if (finalHitRate > 95) finalHitRate = 95;

    int roll = std::rand() % 100;

    if (roll < finalHitRate)
    {
        // 몬스터의 방어력 계산
        // 최종 데미지 = (공격력 × 무기배율) × (1.0 - (방어력 × 0.03))
        int monsterDef = target->GetDef();
        int damage = static_cast<int>(this->atk * (1.0 - (monsterDef * 0.03)));

        target->TakeDamage(damage);

        Render::GetInstance().AddLog(target->GetName() + "에게 " + std::to_string(damage) + "의 데미지!", CLR_WHITE);

        if (target->IsDead())
        {
            int monsterExp = target->GetExp();
            this->GainExp(monsterExp);

            Render::GetInstance().AddLog(target->GetName() + "을(를) 처치하여 " + std::to_string(monsterExp) + " EXP 획득!", CLR_YELLOW);
        }

        return true;
    }
    else
    {
        Render::GetInstance().AddLog(target->GetName() + "에게 공격이 빗나갔습니다!", CLR_DARK_GRAY);
        return false;
    }
}

void Player::Move(GameAction _action, Map& _map)
{
    switch (_action)
    {
    case GameAction::MoveUp:
        TileCheck(0, -1, _map);
        break;

    case GameAction::MoveDown:
        TileCheck(0, 1, _map);
        break;

    case GameAction::MoveLeft:
        TileCheck(-1, 0, _map);
        break;

    case GameAction::MoveRight:
        TileCheck(1, 0, _map);
        break;
    }
}

bool Player::TileCheck(int _x, int _y, Map& _map)
{
    int nextX = x + _x;
    int nextY = y + _y;

    if (_map.GetTile(nextX, nextY) == Tile::Floor)
    {
        _map.SetTile(x, y, Tile::Floor);

        x = nextX;
        y = nextY;

        _map.SetTile(x, y, Tile::Player);
        return true;
    }

    else return false;
}

void Player::OnCollision(Tile targetTile)
{
    switch (targetTile)
    {
    case Tile::Monster:
        // 배틀 매니저
        break;
    case Tile::EliteMonster:
        // 배틀 매니저
        break;
    case Tile::Boss:
        // 배틀 매니저
        break;
    case Tile::Chest:
        // 아이템 매니저
        break;
    case Tile::Stair:
        // 다음 층이동
        break;
    }
}

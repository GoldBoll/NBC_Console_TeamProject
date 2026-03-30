#include "Player.h"
#include "../Monster/Monster.h"
#include "../Render/Render.h"
#include <algorithm>

Player::Player(std::string _jobName)
{
    jobName = _jobName;
    hp = 10;
    maxHp = 10;
    atk = 2;
    dex = 1;

    level = 1;
    exp = 0;
    expToNext = 20;

    maxHpBonus = 2;
    atkBonus = 3;
    dexBonus = 1;
    hpBonus = 5;
}

// Setter
void Player::SetLevel(int _level) { level = _level; }
void Player::SetHp(int _hp) { hp = _hp; }
void Player::SetAtk(int _atk) { atk = _atk; }
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

void Player::TakeDamage(int _damage)
{
    hp -= _damage;
}

Player::~Player() { }

void Player::DetectMonsters(std::vector<Monster*>& _monsters)
{
    for (Monster* monster : _monsters)
    {
        if (monster == nullptr) continue;

        MonsterState oldState = monster->GetState();
        int dist = std::max(std::abs(x - monster->GetX()), std::abs(y - monster->GetY()));

        if (dist <= combatRange)      monster->SetState(MonsterState::COMBAT);
        else if (dist <= chaseRange) monster->SetState(MonsterState::CHASE);
        else                          monster->SetState(MonsterState::IDLE);

        MonsterState newState = monster->GetState();

        if (oldState != newState)
        {
            if (newState == MonsterState::CHASE)
                Render::GetInstance().AddLog(monster->GetName() + "가 당신을 발견했습니다!", CLR_YELLOW);
            else if (newState == MonsterState::COMBAT)
                Render::GetInstance().AddLog(monster->GetName() + "와 전투를 시작합니다!", CLR_RED);
            else if (newState == MonsterState::IDLE && oldState != MonsterState::IDLE)
                Render::GetInstance().AddLog(monster->GetName() + "가 추적을 포기했습니다.", CLR_DARK_GRAY);
        }
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

#include "Player.h"
#include <iostream>

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

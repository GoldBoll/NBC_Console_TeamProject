#include "Player.h"
#include <iostream>
using namespace std;

Player::Player() {

    hp = 10;
    maxHp = 10;
    atk = 2;
    dex = 1;

    level = 1;
    exp = 0;
    expToNext = 20;
}


// Setter
void Player::SetLevel(int _level) { this->hp = _level; }
void Player::SetHp(int _hp) { this->hp = _hp; }
void Player::SetAtk(int _atk) { this->hp = _atk; }
void Player::SetDex(int _dex) { this->hp = _dex; }

void Player::PrintStatus() const {
    cout << "------------------------------------" << endl;
    cout << "* 현재 능력치" << endl;
    cout << "Lv. " << level << endl;
    cout << "HP: " << hp << endl;
    cout << "공격력: " << atk << endl;
    cout << "민첩: " << dex << endl;
    cout << "경험치: " << exp << endl;
    cout << "다음 레벨까지의 남은 경험치: " << expToNext - exp << endl;
    cout << "------------------------------------" << endl;
}

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
        maxHp += 2;
    case 2:
        atk += 3;
    case 3:
        dex += 1;
    case 4:
        hp += 5;
        if (hp >= maxHp)
            hp = maxHp;
    default:
        break;
    }
}

void Player::TakeDamage(int _damage)
{
    hp -= _damage;
}

bool Player::Die()
{
    return true;
}

string Player::GetJobName()
{
    return jobName;
}


Player::~Player() { }

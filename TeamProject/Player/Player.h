#pragma once
#include <string>
#include <vector>
using namespace std;

struct Map;
struct Monster;

class Player
{
protected:
    // 위치
    int x = 0, y = 0;

    // status
    string job;
    int hp;
    int maxHp;
    int atk;
    int dex;

    int level;
    int exp;
    int expToNext;
    //int gold      = 0;

    // Inventory            inventory;
    // vector<StatusEffect> statusEffects;

public:
    Player();

    virtual ~Player();

    // 나중에 2칸씩 움직이는 캐릭터도 있을 수 있으니 일단 가상함수로 구현
    virtual void TryMove(int _dx, int _dy, Map& _map) = 0;

    //virtual void attack() = 0;
    //virtual void attack(Monster* monster) = 0;

    // Getter
    string GetJobName();
    int GetLevel();
    int GetHP();
    int GetAtk();
    int GetDex();

    // Setter
    void SetLevel(int level);
    void SetHp(int hp);
    void SetAtk(int atk);
    void SetDex(int dex);

    // player info
    bool IsAlive() const { return hp > 0; }
    void PrintStatus() const;

    void GainExp(int _exp);
    void LevelUp();
    void TakeDamage(int _damage);
    bool Die();
};

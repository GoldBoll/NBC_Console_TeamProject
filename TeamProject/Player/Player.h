#pragma once
#include "../Item/Inventory.h"
#include "../Battle/StatusEffect.h"
#include <string>
#include <vector>
using namespace std;

struct Map;
struct Monster;

// ── 플레이어 ──────────────────────────────────────────
struct Player
{
    // 위치
    int x = 0, y = 0;

    // 기본 정보
    string name;

    // 전투 스탯
    // text-rpg: defaultHealth = baseHp + constitution*10
    int hp        = 200;
    int maxHp     = 200;
    int atk       = 30;
    int def       = 0;

    // 성장 (text-rpg experience 시스템 참고)
    int level     = 1;
    int exp       = 0;
    int expToNext = 100;
    int gold      = 0;

    // 인벤토리 & 상태이상
    Inventory            inventory;
    vector<StatusEffect> statusEffects;

    // ── 행동 ────────────────────────────────────────
    void tryMove(int dx, int dy, Map& map, vector<Monster>& monsters);
    void gainExp(int amount);
    void levelUp();
    void tickStatus();

    bool isAlive() const { return hp > 0; }
    void printStatus() const;
};

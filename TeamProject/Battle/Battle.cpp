#include "Battle.h"
#include "../Player/Player.h"
#include "../Monster/Monster.h"
#include "../Render/Render.h"
#include <vector>
#include <algorithm>
#include <cmath>

Battle::Battle() = default;
Battle::~Battle() = default;

void Battle::CheckCombat(Player* player, std::vector<Monster*>& monsters)
{
    if (!player || !player->IsAlive()) return;

    for (Monster* monster : monsters)
    {
        if (!monster || monster->IsDead()) continue;

        int dist = static_cast<int>(std::max(std::abs(player->GetX() - monster->GetX()), std::abs(player->GetY() - monster->GetY())));

        MonsterState oldState = monster->GetState();
        MonsterState newState = oldState;

        // 거리가 0일 때만 전투(COMBAT), 그 외 4칸 이내면 추격(CHASE)
        if (dist == 0)      newState = MonsterState::COMBAT;
        else if (dist <= 4) newState = MonsterState::CHASE;
        else                newState = MonsterState::IDLE;

        if (oldState != newState)
        {
            monster->SetState(newState);

            std::string mName = monster->GetName();
            if (newState == MonsterState::COMBAT)
                Render::GetInstance().AddLog("!전투! " + mName, CLR_RED);
            else if (newState == MonsterState::CHASE)
                Render::GetInstance().AddLog("!발견! " + mName, CLR_YELLOW);
            else if (newState == MonsterState::IDLE && oldState == MonsterState::CHASE)
                Render::GetInstance().AddLog("?놓침? " + mName, CLR_DARK_GRAY);
        }
    }
}

void Battle::ProcessTurn(Player* player, std::vector<Monster*>& monsters)
{
    if (!player || !player->IsAlive()) return;

    // 몬스터와 같은 칸에 있을 때만 서로 공격을 주고받음
    for (Monster* monster : monsters)
    {
        if (!monster || monster->IsDead() || monster->GetState() != MonsterState::COMBAT) continue;

        int dist = static_cast<int>(std::max(std::abs(player->GetX() - monster->GetX()), std::abs(player->GetY() - monster->GetY())));

        if (dist == 0)
        {
            // 플레이어 공격
            player->Attack(monster);

            // 몬스터가 반격 (죽지 않았을 때만)
            if (!monster->IsDead())
            {
                monster->Attack(player);
            }
        }
    }
}

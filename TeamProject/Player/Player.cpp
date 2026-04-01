#include "Player.h"
#include "../Monster/Monster.h"
#include "../Render/Render.h"
#include <algorithm>
#include <cstdlib>

#include "../GameManager/GameManager.h"
#include "../SpawnManager/SpawnManager.h"
#include <conio.h>
#include <cmath>


Player::Player(std::string _jobName)
{
    jobName = _jobName;
    hp      = 30;
    maxHp   = 30;
    atk     = 4;
    def     = 0;
    dex     = 1;

    level     = 1;
    exp       = 0;
    expToNext = 20;

    maxHpBonus = 4; // 선택 시 추가 증가량
    atkBonus   = 1;
    defBonus   = 1;
    dexBonus   = 1;
    hpBonus    = 10; // 회복량 보너스
}

// Setter
void Player::SetLevel(int _level) { level = _level; }
void Player::SetHp(int _hp)       { hp = _hp; }
void Player::SetAtk(int _atk)     { atk = _atk; }
void Player::SetDef(int _def)     { def = _def; }
void Player::SetDex(int _dex)     { dex = _dex; }

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

void Player::TickInvisibility()
{
    if (invisibilityTurns <= 0) return;
    --invisibilityTurns;
    if (invisibilityTurns == 0)
    Render::GetInstance().AddLog("투명화가 해제됐습니다.", CLR_DARK_GRAY);
}

void Player::Heal(int amount)
{
    hp += amount;
    if (hp > maxHp) hp = maxHp;
    Render::GetInstance().AddLog("체력이 " + std::to_string(amount) + " 회복됐습니다! (" + std::to_string(hp) + "/" + std::to_string(maxHp) + ")", CLR_GREEN);
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
    level++;

    // 1. 기획서 기반 자동 성장 적용 (최대 20 제한)
    maxHp += 3;
    hp += 3;
    atk += 1;
    if (level % 2 == 0 && def < 20) def += 1;
    if (level % 3 == 0 && dex < 20) dex += 1;

    Render& render = Render::GetInstance();
    render.AddLog(" [ LEVEL UP! " + std::to_string(level) + " ] ", CLR_YELLOW);
    render.AddLog("추가 보너스 스탯을 선택하세요 (1-5):", CLR_CYAN);

    // UI에 현재 스탯이 만렙인지 표시
    std::string defLabel = (def >= 20) ? "DEF(MAX)" : "DEF(+1)";
    std::string dexLabel = (dex >= 20) ? "DEX(MAX)" : "DEX(+1)";

    render.AddLog("1: MaxHP(+4) 2: ATK(+1)", CLR_GRAY);
    render.AddLog("3: " + defLabel + " 4: " + dexLabel + " 5: Heal(55%)", CLR_GRAY);
    render.RenderLog();

    int input = 0;
    while (true)
    {
        if (_kbhit())
        {
            int ch = _getch();
            if (ch >= '1' && ch <= '5') { input = ch - '0'; break; }
        }
    }

    std::string statName = "";
    bool isMax = false;

    switch (input)
    {
    case 1:
        maxHp += maxHpBonus;
        hp += maxHpBonus;
        statName = "MaxHP(+4)";
        break;
    case 2:
        atk += atkBonus;
        statName = "Attack(+1)";
        break;
    case 3:
        if (def < 20) { def += defBonus; statName = "Defense(+1)"; }
        else { isMax = true; statName = "Defense"; }
        break;
    case 4:
        if (dex < 20) { dex += dexBonus; statName = "Dexterity(+1)"; }
        else { isMax = true; statName = "Dexterity"; }
        break;
    case 5:
        int healAmount = static_cast<int>(std::round(maxHp * 0.55f));
        hp += healAmount;
        if (hp >= maxHp) hp = maxHp;
        statName = "Heal(55%)";
        break;
    }

    if (isMax)
        render.AddLog(statName + " 은(는) 이미 최대치(20)입니다!", CLR_RED);
    else
        render.AddLog(statName + " 보너스가 적용되었습니다!", CLR_GREEN);
}

void Player::TakeDamage(int _damage, int attackerDex)
{
    // 명중률 계산: 무기 명중률 + (공격자 DEX - 방어자 DEX) × 2
    int finalHitRate = 80 + (attackerDex - dex) * 2;
    if (finalHitRate < 5) finalHitRate = 5;
    else if (finalHitRate > 95) finalHitRate = 95;

    int roll = std::rand() % 100;

    if (roll < finalHitRate)
    {
        // 명중 시 방어력 계산 적용
        // 최종 데미지 = (공격력 × 무기배율) × (1.0 - (방어력 × 0.03))
        int damageAfterDef = static_cast<int>(std::round(_damage * (1.0 - (def * 0.03))));

        hp -= damageAfterDef;
        Render::GetInstance().AddLog(std::to_string(damageAfterDef) + "의 데미지를 입었습니다!", CLR_RED);

        if (hp <= 0 && hasReviveToken)
        {
            hasReviveToken = false;
            hp = maxHp / 2;
            Render::GetInstance().AddLog("부활 토큰 발동! 체력 " + std::to_string(hp) + "으로 부활!", CLR_YELLOW);
            Render::GetInstance().ClearInfo2();
        }
    }
    else
    {
        Render::GetInstance().AddLog("공격을 회피했습니다!", CLR_WHITE);
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

    // 공격 시도 시, 수치 계산 없이 전달만 함 (TakeDamage에서 통합 처리)
    target->TakeDamage(this->atk, this->dex);

    if (target->IsDead())
    {
        int monsterExp = target->GetExp();

        Render::GetInstance().AddLog(target->GetName() + "을(를) 처치하여 " + std::to_string(monsterExp) + " EXP 획득!", CLR_YELLOW);
        GetInventory().AddItem(ItemManager::GetInstance().CreateRandomItem());

        this->GainExp(monsterExp);
    }

    return true;
}

bool Player::Move(GameAction _action, Map& _map)
{
    int dx = 0, dy = 0;
    switch (_action)
    {
        case GameAction::MoveUp:    dy = -1; break;
        case GameAction::MoveDown:  dy = 1;  break;
        case GameAction::MoveLeft:  dx = -1; break;
        case GameAction::MoveRight: dx = 1;  break;
        default: return false;
    }

    return TileCheck(dx, dy, _map);
}

bool Player::TileCheck(int _dx, int _dy, Map& _map)
{
    int nextX = x + _dx;
    int nextY = y + _dy;

    if (!_map.InBounds(nextX, nextY)) return false;

    Tile targetTile = _map.GetTile(nextX, nextY);

    // 몬스터 타일일 경우: 이동하지 않고 충돌 처리 (겹침 방지)
    if (targetTile == Tile::Monster || targetTile == Tile::EliteMonster || targetTile == Tile::Boss)
    {
        OnCollision(targetTile, nextX, nextY, _map);
        return true;
    }

    // 이동 가능한 타일들 (빈 공간, 문, 계단, 상자 등)
    if (targetTile == Tile::Floor || targetTile == Tile::Door || targetTile == Tile::Stair || targetTile == Tile::Chest)
    {
        _map.SetTile(x, y, Tile::Floor);
        x = nextX;
        y = nextY;
        _map.SetTile(x, y, Tile::Player);

        OnCollision(targetTile, nextX, nextY, _map);
        return true;
    }

    // 이동 불가 타일 (벽, 바위, 물 등)
    // 벽에 부딪혀서 이동은 못했지만 턴이 지나간 것처럼 처리하기 위해 true 반환
    if (targetTile == Tile::Wall || targetTile == Tile::Rock || targetTile == Tile::Water)
    {
        return true;
    }

    return false;
}

void Player::OnCollision(Tile targetTile, int targetX, int targetY, Map& _map)
{
    Render& render = Render::GetInstance();
    switch (targetTile)
    {
        case Tile::Monster:
        // 배틀 매니저
        render.AddLog("몬스터", CLR_DARK_GRAY);
        break;
        case Tile::EliteMonster:
        // 배틀 매니저
        render.AddLog("엘리트 몬스터", CLR_DARK_GRAY);
        break;
        case Tile::Boss:
        // 배틀 매니저
        render.AddLog("보스", CLR_DARK_GRAY);
        break;
        case Tile::Chest:
        // 아이템 매니저
        {
            render.AddLog("보물상자", CLR_DARK_GRAY);
            Monster* m = SpawnManager::GetInstance()->GetMonsterAt(targetX, targetY);
            if (m != nullptr)
            {
                m->Interact();
                _map.SetTile(targetX, targetY, Tile::Monster);
                return;
            }

            // 아이템 상자 획득 처리해야함
            _map.SetTile(targetX, targetY, Tile::Floor);
            Item* item = ItemManager::GetInstance().CreateRandomItem();
            if (item)
            {
                std::string itemName = item->GetName();
                inventory.AddItem(item);
                render.AddLog("보물상자에서 [" + itemName + "] 을(를)", CLR_YELLOW);
                render.AddLog(" 획득했습니다!", CLR_YELLOW);
            }
        }
        break;
        case Tile::Stair:
        // 다음 층이동
        Render::GetInstance().AddLog("계단", CLR_DARK_GRAY);
        GameManager::GetInstance().SetCurrentStageIndex(GameManager::GetInstance().GetCurrentStageIndex() + 1);
        GameManager::GetInstance().Init( GameManager::GetInstance().GetCurrentStageIndex() );
        break;
    }
}

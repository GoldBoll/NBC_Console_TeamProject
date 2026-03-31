#include "Player.h"
#include "../Monster/Monster.h"
#include "../Render/Render.h"
#include <algorithm>
#include <cstdlib>

#include "../GameManager/GameManager.h"

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

    // 공격 시도 시, 수치 계산 없이 전달만 함 (TakeDamage에서 통합 처리)
    target->TakeDamage(this->atk, this->dex);

    if (target->IsDead())
    {
        int monsterExp = target->GetExp();
        this->GainExp(monsterExp);

        Render::GetInstance().AddLog(target->GetName() + "을(를) 처치하여 " + std::to_string(monsterExp) + " EXP 획득!", CLR_YELLOW);
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
        OnCollision(targetTile);
        return true;
    }

    // 이동 가능한 타일들 (빈 공간, 문, 계단, 상자 등)
    if (targetTile == Tile::Floor || targetTile == Tile::Door || targetTile == Tile::Stair || targetTile == Tile::Chest)
    {
        _map.SetTile(x, y, Tile::Floor);
        x = nextX;
        y = nextY;
        _map.SetTile(x, y, Tile::Player);

        OnCollision(targetTile);
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

void Player::OnCollision(Tile targetTile)
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
        render.AddLog("보물상자", CLR_DARK_GRAY);
        break;
    case Tile::Stair:
        // 다음 층이동
        Render::GetInstance().AddLog("계단", CLR_DARK_GRAY);
        GameManager::GetInstance().SetCurrentStageIndex(GameManager::GetInstance().GetCurrentStageIndex() + 1);
        GameManager::GetInstance().Init( GameManager::GetInstance().GetCurrentStageIndex() );
        break;
    }
}

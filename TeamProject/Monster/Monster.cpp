#include "Monster.h"
#include "../Render/Render.h"
#include <cstdlib>

Monster::Monster(std::string _name, int _hp, int _atk, int _def, int _dex, int _exp, bool _elite)
    :name(_name), hp(_hp), maxHp(_hp), atk(_atk), def(_def), dex(_dex), exp(_exp), x(0), y(0), isElite(_elite)
{
    if (isElite)
    {
        name = "[Elite]" + _name;
    }
}

void Monster::TakeDamage(int _damage, int attackerDex)
{
    // 명중률 계산: 무기 명중률 + (공격자 DEX - 방어자 DEX) × 2
    int attackerWeaponHit = 90;
    int finalHitRate = attackerWeaponHit + (attackerDex - dex) * 2;
    if (finalHitRate < 5) finalHitRate = 5;
    else if (finalHitRate > 95) finalHitRate = 95;

    int roll = std::rand() % 100;

    if (roll < finalHitRate)
    {
        // 방어력 계산 적용
        // 최종 데미지 = 공격력 × (1.0 - (방어력 × 0.03))
        int damageAfterDef = static_cast<int>(_damage * (1.0 - (def * 0.03)));

        hp -= damageAfterDef;
        Render::GetInstance().AddLog(name + "에게 " + std::to_string(damageAfterDef) + "의 데미지!", CLR_WHITE);
    }
    else
    {
        Render::GetInstance().AddLog(name + "에게 공격이 빗나갔습니다!", CLR_DARK_GRAY);
    }
}

void Monster::Move(int _dx, int _dy, Map& _map)
{
    int nextX = x + _dx;
    int nextY = y + _dy;

    // 현재 위치와 다음 위치가 모두 유효해야 동작
    if (_map.InBounds(nextX, nextY) && _map.GetTile(nextX, nextY) == Tile::Floor)
    {
        //Tile currentTile = _map.GetTile(x, y);

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
    //if (state == MonsterState::CHASE || state == MonsterState::COMBAT) return;
    if (state != MonsterState::IDLE) return;

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
    // 이동 가능한 4방향 (상, 하, 좌, 우)
    struct Dir { int dx, dy; int dist; };
    Dir dirs[4] = { {0, -1, 0}, {0, 1, 0}, {-1, 0, 0}, {1, 0, 0} };

    // 각 방향으로 이동했을 때의 플레이어와의 거리 계산
    for (int i = 0; i < 4; ++i) {
        int nx = x + dirs[i].dx;
        int ny = y + dirs[i].dy;
        // 맨해튼 거리 사용 (가로나 세로 합)
        dirs[i].dist = std::abs(targetX - nx) + std::abs(targetY - ny);
    }

    // 거리 순으로 정렬 (가장 가까워지는 방향이 0번 인덱스로 오도록)
    for (int i = 0; i < 3; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            if (dirs[i].dist > dirs[j].dist) {
                Dir temp = dirs[i];
                dirs[i] = dirs[j];
                dirs[j] = temp;
            }
        }
    }

    // 플레이어와 가장 가까워지는 방향부터 시도하여 갈 수 있는 곳(Floor)으로 이동
    for (int i = 0; i < 4; ++i) {
        int nx = x + dirs[i].dx;
        int ny = y + dirs[i].dy;

        if (_map.InBounds(nx, ny) && _map.GetTile(nx, ny) == Tile::Floor) {
            this->Move(dirs[i].dx, dirs[i].dy, _map);
            return; // 이동 성공 시 종료
        }
    }
}

bool Monster::Attack(Player* player)
{
    if (!player) return false;

    // 공격 시도 시, 수치 계산 없이 전달만 함 (TakeDamage에서 통합 처리)
    Render::GetInstance().AddLog(this->GetName() + "의 공격!", CLR_RED);
    player->TakeDamage(this->atk, this->dex);
    return true;
}

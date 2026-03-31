#include "Golem.h"

Golem::Golem(bool _elite)
    : Monster("골렘 ",
             (_elite ? 60 : 50), // HP
             (_elite ? 13 : 9),  // ATK
             (_elite ? 14 : 10), // DEF
             (_elite ? 7  : 5),  // DEX
             (_elite ? 10 : 7),  // EXP
             _elite)
{
    moveCount = 0;
    angry = false;
}

void Golem::Move(int _dx, int _dy, Map& _map)
{
    // 화난 상태 확인 및 2턴 확인
    // 화난 상태일 때는 1턴에 1이동
    if (moveCount < 1 && !angry)
    {
        moveCount++;
        if (state == MonsterState::CHASE)
        {
            angryGauge++;
        }
        return;
    }

    int nextX = x + _dx;
    int nextY = y + _dy;

    // 현재 위치와 다음 위치가 모두 유효해야 동작
    if (_map.InBounds(nextX, nextY) && _map.GetTile(nextX, nextY) == Tile::Floor)
    {
        // 이전 위치 타일 정리
        _map.SetTile(x, y, Tile::Floor);

        x = nextX;
        y = nextY;

        // 자신의 상태(Elite 여부)에 맞는 타일 배치
        if (isElite)
            _map.SetTile(x, y, Tile::EliteMonster);
        else
            _map.SetTile(x, y, Tile::Monster);

        if(!angry) moveCount = 0;
    }
}

Tile Golem::GetDisplayTile() const
{
    return isElite ? Tile::EliteMonster : Tile::Monster;
}

void Golem::AngryGaugeCheck()
{
    if (angryGauge >= 3)
    {
        angry = true;
        angryGauge = 0;
    }
}

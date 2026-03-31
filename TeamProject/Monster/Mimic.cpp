#include "Mimic.h"
#include "../Map/Map.h"
#include "../Render/Render.h"

Mimic::Mimic(bool _elite)
    : Monster("미믹 ",
             (_elite ? 30 : 20), // HP
             (_elite ? 10 : 7),  // ATK
             (_elite ? 10 : 7),  // DEF
             (_elite ? 7  : 5),  // DEX
             (_elite ? 10 : 7),  // EXP
             _elite)
{
    name = "상자";
}

void Mimic::Move(int _dx, int _dy, Map& _map)
{
    // 상자로 변신상태 확인
    if (isDisguised) return;

    // 위장 해제
    Reveal();

    // 2턴 확인
    if (moveCount < 1)
    {
        moveCount++;
        return;
    }

    Monster::Move(_dx, _dy, _map);

    moveCount = 0;
}

// 위장 해제
void Mimic::Reveal()
{
    isDisguised = false;
    name = "미믹";
}

void Mimic::Interact()
{
    if (isDisguised)
    {
        this->Reveal();
    }
}

Tile Mimic::GetDisplayTile() const
{
    return isDisguised ? Tile::Chest : Tile::Monster;
}

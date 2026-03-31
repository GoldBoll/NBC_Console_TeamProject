#include "Goblin.h"

Goblin::Goblin(bool _elite)
    : Monster("고블린 ",
             (_elite ? 15 : 10), // HP
             (_elite ? 5  : 3),  // ATK
             (_elite ? 0  : 0),  // DEF
             (_elite ? 4  : 3),  // DEX
             (_elite ? 8  : 5),  // EXP
             _elite)
{
}

Tile Goblin::GetDisplayTile() const
{
    return isElite ? Tile::EliteMonster : Tile::Monster;
}

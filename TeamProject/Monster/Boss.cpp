#include "Boss.h"

Boss::Boss()
    : Monster("강철 골렘", 200, 20, 20, 10, 100, false)
{
}

Tile Boss::GetDisplayTile() const
{
    return Tile::Boss;
}

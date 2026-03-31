#pragma once
#include "Monster.h"

class Goblin : public Monster
{
public:
    Goblin(bool _elite);
    Tile GetDisplayTile() const override;
};

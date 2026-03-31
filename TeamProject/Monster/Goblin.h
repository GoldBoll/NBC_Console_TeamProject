#pragma once
#include "Monster.h"

class Goblin : public Monster
{
public:
    Goblin(bool _elite);
    virtual Tile GetDisplayTile() const override;
};

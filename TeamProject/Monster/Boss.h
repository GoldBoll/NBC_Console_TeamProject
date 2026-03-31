#pragma once
#include "Monster.h"

class Boss : public Monster
{
public:
    Boss();
    Tile GetDisplayTile() const override;
};


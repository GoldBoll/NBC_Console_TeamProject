#pragma once
#include "Monster.h"

class Orc :public Monster
{
public:
    Orc(bool _elite);
    Tile GetDisplayTile() const override;
};


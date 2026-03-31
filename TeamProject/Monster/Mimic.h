#pragma once
#include "Monster.h"

class Mimic : public Monster
{
private:
    int moveCount = 0;
    bool isDisguised = true;

public:
    Mimic(bool _elite);
    void Move(int _dx, int _dy, Map& _map) override;
    void Reveal();
    void Interact() override;
    Tile GetDisplayTile() const override;
};


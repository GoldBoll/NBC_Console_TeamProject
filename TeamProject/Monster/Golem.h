#pragma once
#include "Monster.h"

class Golem : public Monster
{
private:
    int moveCount = 0;
    int angryGauge = 0;
    bool angry = false;
public:
    Golem(bool _elite);
    void Move(int _dx, int _dy, Map& _map) override;
    Tile GetDisplayTile() const override;

    void AngryGaugeCheck();
    void SetAngry(int _angry) { angry = _angry; };
};


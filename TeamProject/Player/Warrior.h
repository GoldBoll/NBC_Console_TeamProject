#pragma once
#include "Player.h"

class Warrior : public Player
{
public:
    Warrior();
    //void attack() override;
    //void attack(Monster* monster) override;
    void TryMove(int _dx, int _dy, Map& _map) override;
};

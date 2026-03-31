#pragma once
#include <vector>

class Player;
class Monster;

class Battle
{
public:
    Battle();
    ~Battle();

    void ProcessTurn(Player* player, std::vector<Monster*>& monsters);
    void CheckCombat(Player* player, std::vector<Monster*>& monsters);
};

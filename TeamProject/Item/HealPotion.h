#pragma once
#include "Item.h"

// 회복 물약 - 체력 회복
class HealPotion : public Item
{
public:
    HealPotion()
        : Item(101, "회복 물약", "체력을 회복합니다.", ItemType::POTION, true) {}

    void Use(Player& player) override {}
};

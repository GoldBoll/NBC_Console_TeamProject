#pragma once
#include "Item.h"

// 랜덤 이동 물약 - 맵 내 랜덤 위치로 순간이동
class TeleportPotion : public Item
{
public:
    TeleportPotion()
        : Item(102, "랜덤 이동 물약", "맵 내 랜덤 위치로 순간이동합니다.", ItemType::POTION, true) {}

    void Use(Player& player) override {}
};

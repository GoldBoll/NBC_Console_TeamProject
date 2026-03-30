#pragma once
#include "Item.h"

// 음식 - 체력 소량 회복 (전투 외 사용)
class Food : public Item
{
public:
    Food()
        : Item(103, "음식", "체력을 소량 회복합니다. (전투 외 사용)", ItemType::ETC, true) {}

    void Use(Player& player) override {}
};

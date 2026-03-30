#pragma once
#include "Item.h"

// 투명화 아이템 - 일정 턴간 몬스터 탐지 회피
class InvisibilityItem : public Item
{
public:
    InvisibilityItem()
        : Item(104, "투명화 아이템", "일정 턴 동안 몬스터의 탐지를 회피합니다.", ItemType::ETC, true) {}

    void Use(Player& player) override {}
};

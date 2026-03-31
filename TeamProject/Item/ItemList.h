#pragma once
#include "Item.h"
#include "../Player/Player.h"

// ================================================================
// 아이템 목록
// 새 아이템 추가 시 이 파일에 클래스를 직접 추가하세요.
// Item(itemCode, 이름, 설명, ItemType, 소모품여부, UseContext)
// ================================================================

// 101 - 회복 물약 (전투 / 비전투 모두 사용 가능)
class HealPotion : public Item
{
public:
    HealPotion()
        : Item(101, "회복 물약", "체력을 회복합니다.", ItemType::POTION, true, UseContext::BOTH) {}

    void Use(Player& player) override
    {
        player.Heal(10);
    }
};

// 102 - 랜덤 이동 물약 (비전투 중에만 사용 가능)
class TeleportPotion : public Item
{
public:
    TeleportPotion()
        : Item(102, "랜덤 이동 물약", "맵 내 랜덤 위치로 순간이동합니다.", ItemType::POTION, true, UseContext::FIELD_ONLY) {}

    void Use(Player& player) override {}
};

// 103 - 투명화 아이템 (비전투 중에만 사용 가능)
class InvisibilityItem : public Item
{
public:
    InvisibilityItem()
        : Item(103, "투명화 아이템", "일정 턴 동안 몬스터의 탐지를 회피합니다.", ItemType::ETC, true, UseContext::FIELD_ONLY) {}

    void Use(Player& player) override {}
};

// 104 - 보호막 (전투 중에만 사용 가능)
class ShieldItem : public Item
{
public:
    ShieldItem()
        : Item(104, "보호막", "1회 공격을 무시합니다.", ItemType::ETC, true, UseContext::COMBAT_ONLY) {}

    void Use(Player& player) override {}
};

// 105 - 부활 토큰 (전투 중에만 사용 가능)
class ReviveToken : public Item
{
public:
    ReviveToken()
        : Item(105, "부활 토큰", "최대 체력 50%로 1회 부활합니다.", ItemType::ETC, true, UseContext::COMBAT_ONLY) {}

    void Use(Player& player) override {}
};

// 106 - 폭탄 (전투 중에만 사용 가능)
class Bomb : public Item
{
public:
    Bomb()
        : Item(106, "폭탄", "주위 적에게 범위 공격을 가합니다.", ItemType::ETC, true, UseContext::COMBAT_ONLY) {}

    void Use(Player& player) override {}
};

// 107 - 흡혈 (전투 중에만 사용 가능)
class VampireItem : public Item
{
public:
    VampireItem()
        : Item(107, "흡혈", "적에게 입힌 데미지의 일부만큼 체력을 회복합니다.", ItemType::ETC, true, UseContext::COMBAT_ONLY) {}

    void Use(Player& player) override {}
};

// 108 - 스턴 (전투 중에만 사용 가능)
class StunItem : public Item
{
public:
    StunItem()
        : Item(108, "스턴", "주위 적을 1턴 동안 행동 불가 상태로 만듭니다.", ItemType::ETC, true, UseContext::COMBAT_ONLY) {}

    void Use(Player& player) override {}
};

// 109 - 복제 (전투 / 비전투 모두 사용 가능)
class DuplicateItem : public Item
{
public:
    DuplicateItem()
        : Item(109, "복제", "인벤토리의 아이템을 1개 복제합니다.", ItemType::ETC, true, UseContext::BOTH) {}

    void Use(Player& player) override {}
};

// 110 - 분노 (전투 중에만 사용 가능)
class RageItem : public Item
{
public:
    RageItem()
        : Item(110, "분노", "다음 공격의 데미지가 2배가 됩니다.", ItemType::ETC, true, UseContext::COMBAT_ONLY) {}

    void Use(Player& player) override {}
};

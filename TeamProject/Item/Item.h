#pragma once
#include <string>
#include <iostream>

class Player;

enum class ItemType
{
    POTION,
    ETC
};

// 아이템 사용 가능 상황
enum class UseContext
{
    BOTH,         // 전투 / 비전투 모두 사용 가능
    COMBAT_ONLY,  // 전투 중에만 사용 가능
    FIELD_ONLY,   // 비전투 중에만 사용 가능
};

struct Item
{
    int         itemCode;
    std::string name;
    std::string description;
    ItemType    type;
    bool        isConsumable;
    UseContext  useContext;
    int         count;

    Item(int code, std::string n, std::string desc, ItemType t, bool consumable, UseContext ctx)
        : itemCode(code), name(n), description(desc), type(t), isConsumable(consumable), useContext(ctx), count(1) {}

    virtual ~Item() {}

    std::string GetName()        const { return name; }
    std::string GetDescription() const { return description; }

    bool CanUseInCombat() const { return useContext == UseContext::BOTH || useContext == UseContext::COMBAT_ONLY; }
    bool CanUseInField()  const { return useContext == UseContext::BOTH || useContext == UseContext::FIELD_ONLY; }

    virtual void Use(Player& player) {}
};

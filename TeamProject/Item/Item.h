#pragma once
#include <string>
#include <iostream>

class Player;

enum class ItemType
{
    WEAPON,
    POTION,
    ETC
};
struct Item
{
    int itemCode;
    std::string name;
    std::string description;
    ItemType type;
    bool isConsumable;

    int count;

    Item(int code, std::string n, std::string desc, ItemType t, bool consumable)
        : itemCode(code), name(n), description(desc), type(t), isConsumable(consumable), count(1) {}

    virtual ~Item() {}

    std::string GetName()        const { return name; }
    std::string GetDescription() const { return description; }

    virtual void Use(Player& player) {}
};

#pragma once
#include <string>
#include <iostream>

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
    ItemType type;
    bool isConsumable;

    Item(int code, std::string n, ItemType t, bool consuable)
        : itemCode(code), name(n), type(t), isConsumable(consuable) {}

    virtual ~Item() {}

    std::string GetName() const { return name; }

    void Use()
    {
        if (type == ItemType::WEAPON)
        {
            std::cout << "무기를 휘두릅니다." << std::endl;
        }
        else if (type == ItemType::POTION)
        {
            std::cout << "물약을 마십니다." << std::endl;
        }
        else if (type == ItemType::ETC)
        {
            std::cout << "사용할 수 없습니다." << std::endl;
        }
    }
};
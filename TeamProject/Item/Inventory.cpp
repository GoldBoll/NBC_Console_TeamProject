#include "Inventory.h"
#include "../Player/Player.h"

Inventory::Inventory()
{
    for (int i = 0; i < MAX_SLOTS; i++)
    {
        slots[i] = nullptr;
    }
}

Inventory::~Inventory()
{
    for (int i = 0; i < MAX_SLOTS; i++)
    {
        if (slots[i] != nullptr)
        {
            delete slots[i];
        }
    }
}

bool Inventory::AddItem(Item* newItem)
{
    if (newItem == nullptr) return false;

    for (int i = 0; i < MAX_SLOTS; i++)
    {
        if (slots[i] == nullptr)
        {
            slots[i] = newItem;
            //std::cout << "[시스템] 아이템을 획득했습니다! (슬롯: " << i << ")\n";
            return true;
        }
    }
    return false;
}

bool Inventory::UseItem(int index, Player& player)
{
    if (index < 0 || index >= MAX_SLOTS || slots[index] == nullptr)
    {
        //std::cout << "[에러] 사용할 수 있는 아이템이 없습니다.\n";
        return false;
    }
    slots[index]->Use(player);

    if (slots[index]->isConsumable)
    {
        return RemoveItem(index);
    }
    //std::cout << "[시스템] " << index << "번 슬롯의 아이템을 사용했습니다.\n";
    return true;
}

bool Inventory::RemoveItem(int index)
{
    if (index < 0 || index >= 20)
{
        //std::cout << "[에러] 잘못된 인벤토리 번호입니다.\n";
        return false;
    }

    if (slots[index] == nullptr)
{
        //std::cout << "[알림] " << index << "번 칸은 이미 비어 있습니다.\n";
        return false;
    }

    delete slots[index];
    slots[index] = nullptr;

    //std::cout << "[시스템] " << index << "번 슬롯의 아이템을 삭제했습니다.\n";
    return true;
}

void Inventory::ShowInventory() const
{
    std::cout << "\n=== [ 달여우의 인벤토리 ] ===\n";
    for (int i = 0; i < 20; i++)
    {
        if (i < 10) std::cout << "[0" << i << "] ";
        else std::cout << "[" << i << "] ";

        if (slots[i] == nullptr)
        {
            std::cout << "--- (비어 있음) ---" << std::endl;
        }
        else {
            std::cout << "★ " << slots[i]->GetName() << std::endl;
        }
    }
    std::cout << "================================\n" << std::endl;
}

std::vector<std::pair<int, Item*>> Inventory::GetFilledItems() const
{
    std::vector<std::pair<int, Item*>> result;
    for (int i = 0; i < MAX_SLOTS; ++i)
        if (slots[i] != nullptr)
            result.push_back({ i, slots[i] });
    return result;
}

#pragma once
#include "ItemList.h"
#include "../GameManager/InputManager.h"

class Player;

class ItemManager
{
public:
    static ItemManager& GetInstance();

    ItemManager(const ItemManager&)            = delete;
    ItemManager& operator=(const ItemManager&) = delete;

    // 아이템 생성 (전체 중 랜덤 / 타입 지정 랜덤)
    Item* CreateRandomItem();
    Item* CreateItemByType(ItemType type);

    // 인벤토리 UI 관리
    void OpenInventory(Player* player);
    void CloseInventory(Player* player);
    void HandleInventoryAction(GameAction action, Player* player, bool isBattleMode);

    bool IsOpen() const { return inventoryOpen; }

private:
    ItemManager() = default;

    bool inventoryOpen   = false;
    int  invSelectedIdx  = 0;
    int  invScrollOffset = 0;
};

#include "ItemManager.h"
#include "../Player/Player.h"
#include "../Render/Render.h"
#include "../Render/UILayout.h"
#include <functional>
#include <vector>
#include <cstdlib>

ItemManager& ItemManager::GetInstance()
{
    static ItemManager instance;
    return instance;
}

// 아이템 생성

Item* ItemManager::CreateRandomItem()
{
    static const std::vector<std::function<Item*()>> allItems =
    {
        [] { return new HealPotion();      },  // 101
        [] { return new TeleportPotion();  },  // 102
        [] { return new InvisibilityItem();},  // 103
        //[] { return new ShieldItem();      },  // 104 - 미구현
        [] { return new ReviveToken();     },  // 105
        //[] { return new Bomb();            },  // 106 - 미구현
        //[] { return new VampireItem();     },  // 107 - 미구현
        //[] { return new StunItem();        },  // 108 - 미구현
        //[] { return new DuplicateItem();   },  // 109 - 미구현
        //[] { return new RageItem();        },  // 110 - 미구현
    };
    return allItems[rand() % allItems.size()]();
}


//나중에 몬스터 드랍 아이템이나 상점 아이템 생성할 때 타입별로 랜덤 생성이 필요할 수 있어서 미리 구현해둠
Item* ItemManager::CreateItemByType(ItemType type)
{
    if (type == ItemType::POTION)
    {
        static const std::vector<std::function<Item*()>> potions =
        {
            [] { return new HealPotion();     },
            [] { return new TeleportPotion(); },
        };
        return potions[rand() % potions.size()]();
    }
    else // ETC
    {
        static const std::vector<std::function<Item*()>> etc =
        {
            [] { return new InvisibilityItem(); },  // 103
            //[] { return new ShieldItem();       },  // 104 - 미구현
            [] { return new ReviveToken();      },  // 105
            //[] { return new Bomb();             },  // 106 - 미구현
            //[] { return new VampireItem();      },  // 107 - 미구현
            //[] { return new StunItem();         },  // 108 - 미구현
            //[] { return new DuplicateItem();    },  // 109 - 미구현
            //[] { return new RageItem();         },  // 110 - 미구현
        };
        return etc[rand() % etc.size()]();
    }
}

Item* ItemManager::CreateItemByCode(int code)
{
    switch (code)
    {
    case 101: return new HealPotion();
    case 102: return new TeleportPotion();
    case 103: return new InvisibilityItem();
    //case 104: return new ShieldItem();      // 미구현
    case 105: return new ReviveToken();
    //case 106: return new Bomb();            // 미구현
    //case 107: return new VampireItem();     // 미구현
    //case 108: return new StunItem();        // 미구현
    //case 109: return new DuplicateItem();   // 미구현
    //case 110: return new RageItem();        // 미구현
    default:  return nullptr;
    }
}

// 인벤토리 UI

void ItemManager::OpenInventory(Player* player)
{
    inventoryOpen   = true;
    invSelectedIdx  = 0;
    invScrollOffset = 0;

    Render& render = Render::GetInstance();
    render.RenderInventory(player->GetInventory(), invSelectedIdx, invScrollOffset);

    auto items = player->GetInventory().GetFilledItems();
    const Item* sel = items.empty() ? nullptr : items[0].second;
    render.RenderItemDesc(sel);
}

void ItemManager::CloseInventory(Player* player)
{
    inventoryOpen   = false;
    invSelectedIdx  = 0;
    invScrollOffset = 0;

    Render& render = Render::GetInstance();
    render.RenderInfo(player);

    if (player->HasReviveToken())
        render.ShowReviveToken();
    else
        render.ClearInfo2();
}

void ItemManager::HandleInventoryAction(GameAction action, Player* player, bool isBattleMode)
{
    Render& render        = Render::GetInstance();
    const int maxVisible  = PLAYER_INFO_H - 4;
    auto items            = player->GetInventory().GetFilledItems();

    switch (action)
    {
    case GameAction::MoveUp:
        if (invSelectedIdx > 0)
        {
            --invSelectedIdx;
            if (invSelectedIdx < invScrollOffset)
                --invScrollOffset;
        }
        break;

    case GameAction::MoveDown:
        if (invSelectedIdx < (int)items.size() - 1)
        {
            ++invSelectedIdx;
            if (invSelectedIdx >= invScrollOffset + maxVisible)
                ++invScrollOffset;
        }
        break;

    case GameAction::UseItem:
        if (!items.empty() && invSelectedIdx < (int)items.size())
        {
            int         slotIdx  = items[invSelectedIdx].first;
            Item*       selItem  = items[invSelectedIdx].second;
            std::string itemName = selItem->GetName();

            if (isBattleMode && !selItem->CanUseInCombat())
            {
                render.AddLog("[" + itemName + "] 전투 중에는 사용할 수 없습니다.", CLR_RED);
                break;
            }
            if (!isBattleMode && !selItem->CanUseInField())
            {
                render.AddLog("[" + itemName + "] 전투 중에만 사용할 수 있습니다.", CLR_RED);
                break;
            }

            player->GetInventory().UseItem(slotIdx, *player);
            render.AddLog(itemName + " 사용!", CLR_GREEN);

            // 소모 후 선택 인덱스 보정
            auto updated = player->GetInventory().GetFilledItems();
            if (invSelectedIdx >= (int)updated.size())
                invSelectedIdx = (int)updated.size() - 1;
            if (invSelectedIdx < 0) invSelectedIdx = 0;
            if (invScrollOffset > invSelectedIdx)
                invScrollOffset = invSelectedIdx;
        }
        break;

    default:
        break;
    }

    // 패널 갱신
    auto updated = player->GetInventory().GetFilledItems();
    render.RenderInventory(player->GetInventory(), invSelectedIdx, invScrollOffset);
    const Item* sel = (!updated.empty() && invSelectedIdx < (int)updated.size())
        ? updated[invSelectedIdx].second : nullptr;
    render.RenderItemDesc(sel);
    render.RenderLog();
}

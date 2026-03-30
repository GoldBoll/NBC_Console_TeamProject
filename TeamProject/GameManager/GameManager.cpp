#include "GameManager.h"
#include "../Render/Render.h"
#include "../Player/Player.h"
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include "../Item/HealPotion.h"
#include "../Item/InvisibilityItem.h"
#include "../Item/TeleportPotion.h"
#include "../SpawnManager/SpawnManager.h"

GameManager& GameManager::GetInstance()
{
    static GameManager instance;
    return instance;

}

GameManager::GameManager()
{
    player = new Warrior();
}

void GameManager::Init()
{
    srand((unsigned int)time(NULL));

    Render& render = Render::GetInstance();
    render.Init("TextRPG - NBC Team Project 2");

    //아이템 출력확인
    player->GetInventory().AddItem(new HealPotion());
    player->GetInventory().AddItem(new InvisibilityItem());
    player->GetInventory().AddItem(new TeleportPotion());

    // BSP 맵 생성
    BspManager::Params bspParams;
    // bspParams.seed     = 12345;  // 고정 시드가 필요하면 주석 해제
    // bspParams.maxDepth = 4;      // 깊이 줄이면 방이 크고 적어짐
    // bspParams.minPartitionW = 40; bspParams.minPartitionH = 40; // 큰 방
    BspManager::GetInstance().Generate(map, bspParams);

    const std::vector<Room>& rooms = BspManager::GetInstance().GetRooms();

    // 플레이어 스폰: 첫 번째 방 중앙
    if (!rooms.empty())
    {
        player->SetX(rooms.front().CenterX());
        player->SetY(rooms.front().CenterY());
    }

    // 계단: 마지막 방 중앙
    if (rooms.size() >= 2)
    {
        map.SetTile(rooms.back().CenterX(), rooms.back().CenterY(), Tile::Stair);
    }

    SpawnManager::GetInstance()->SpawnMonstersInRooms(map, rooms);
    monsters = SpawnManager::GetInstance()->GetActiveMonsters();

    render.DrawStaticUI();
    render.RenderHelp();

    render.AddLog("Game started!",              CLR_YELLOW);
    render.AddLog("WASD / arrow keys to move.", CLR_GRAY);

    running     = true;
    needsRedraw = true;

    render.RenderMap(map, player);
    render.RenderInfo(player);
    render.RenderLog();
    /*for (int y = 0; y < MAP_H; ++y)
    {
        for (int x = 0; x < MAP_W; ++x)
        {
            if (map.GetTile(x, y) == Tile::Monster)
            {
                Monster* monster = new Monster("Slime", 10, 1, 1, 10);
                monster->SetPosition(x, y);
                monsters.push_back(monster);
                map.SetTile(x, y, Tile::Floor);
            }
        }
    }*/
}

void GameManager::Run()
{
    Init();

    Render&       render = Render::GetInstance();
    InputManager& input  = InputManager::GetInstance();

    while (running)
    {
        GameAction action = input.PollInput();
        if (action != GameAction::None)
        {
            HandleAction(action);

            if (inputbutton)
            {
                UpdateMonster();
            }
            needsRedraw = true;
        }

        if (needsRedraw)
        {
            render.RenderMap(map, player);
            if (!inventoryOpen)
                render.RenderInfo(player);
            render.RenderLog();
            needsRedraw = false;
        }
    }

    system("cls");
    std::cout << "Game over. Thanks for playing!\n";
}

void GameManager::HandleAction(GameAction action)
{
    Render& render = Render::GetInstance();

    switch (action)
    {
        case GameAction::MoveUp:
        if (inventoryOpen) { HandleInventoryAction(action); break; }
        render.AddLog("moved up",    CLR_DARK_GRAY);
        player->Move(action, map);
        inputbutton = true;
        break;

        case GameAction::MoveDown:
        if (inventoryOpen) { HandleInventoryAction(action); break; }
        render.AddLog("moved down",  CLR_DARK_GRAY);
        player->Move(action, map);
        inputbutton = true;
        break;

        case GameAction::MoveLeft:
        if (inventoryOpen) { HandleInventoryAction(action); break; }
        render.AddLog("moved left",  CLR_DARK_GRAY);
        player->Move(action, map);
        inputbutton = true;
        break;

        case GameAction::MoveRight:
        if (inventoryOpen) { HandleInventoryAction(action); break; }
        render.AddLog("moved right", CLR_DARK_GRAY);
        player->Move(action, map);
        inputbutton = true;
        break;

        case GameAction::Help:
        render.AddLog("WASD/arrows: move  |  h: help  |  q: quit", CLR_CYAN);
        break;

        case GameAction::Quit:
        running = false;
        break;

        case GameAction::Inventory:
            if (inventoryOpen) CloseInventory();
            else               OpenInventory();
            break;

        case GameAction::UseItem:
            if (inventoryOpen) HandleInventoryAction(action);
            break;

        default:
            break;
    }
    //if (moved)
    //{
    //    // 1. DetectMonsters의 인자 타입을 포인터 벡터 버전으로 맞춰줘야 해! (아래 팁 참고)
    //    player->DetectMonsters(monsters);
    //
    //    // 2. 반복문에서 Monster* (포인터)를 꺼내야 해
    //    for (Monster* monster : monsters)
    //    {
    //        if (monster == nullptr) continue; // 안전장치!
    //
    //        // 3. 포인터니까 점(.)이 아니라 화살표(->)를 써야 해
    //        monster->Update(player->GetX(), player->GetY());
    //    }
    //    needsRedraw = true;
    //}
}

void GameManager::UpdateMonster()
{
    for (Monster* monster : monsters)
    {
        if (monster == nullptr || monster->IsDead()) continue;

        monster->UpdateAI(map);
    }
}

void GameManager::OpenInventory()
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

void GameManager::CloseInventory()
{
    inventoryOpen   = false;
    invSelectedIdx  = 0;
    invScrollOffset = 0;

    Render& render = Render::GetInstance();
    render.RenderInfo(player);
    render.ClearInfo2();
}

void GameManager::HandleInventoryAction(GameAction action)
{
    Render& render  = Render::GetInstance();
    const int maxVisible = PLAYER_INFO_H - 4;
    auto items = player->GetInventory().GetFilledItems();

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
            int   slotIdx  = items[invSelectedIdx].first;
            std::string itemName = items[invSelectedIdx].second->GetName();
            player->GetInventory().UseItem(slotIdx, *player);
            render.AddLog("Used: " + itemName, CLR_GREEN);

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

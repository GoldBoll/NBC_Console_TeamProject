#include "GameManager.h"
#include "../BspManager/BspManager.h"
#include "../Render/Render.h"
#include "../Player/Player.h"
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "../Item/HealPotion.h"
#include "../Item/InvisibilityItem.h"
#include "../Item/TeleportPotion.h"
#include "../SpawnManager/SpawnManager.h"

// 수동 클램프 함수
template<typename T>
T ClampValue(T val, T minVal, T maxVal)
{
    if (val < minVal) return minVal;
    if (val > maxVal) return maxVal;
    return val;
}

GameManager& GameManager::GetInstance()
{
    static GameManager instance;
    return instance;
}

GameManager::GameManager()
{
    player = new Warrior();
}

void GameManager::Init(int stage)
{
    srand((unsigned int)time(NULL));

    Render& render = Render::GetInstance();
    render.Init("TextRPG - NBC Team Project 2");

    //아이템 출력확인
    player->GetInventory().AddItem(new HealPotion());
    player->GetInventory().AddItem(new HealPotion());
    player->GetInventory().AddItem(new InvisibilityItem());
    player->GetInventory().AddItem(new TeleportPotion());

    // BSP 맵 생성
    BspManager::Params bspParams;
    BspManager::GetInstance().Generate(map, bspParams);

    const std::vector<Room>& rooms = BspManager::GetInstance().GetRooms();

    // 플레이어 스폰
    SpawnManager::GetInstance()->SpawnPlayerInRooms(map, rooms, player);

    // 오브젝트 스폰
    SpawnManager::GetInstance()->SpawnObjectInRooms(map, rooms);

    // 몬스터 스폰
    SpawnManager::GetInstance()->SpawnMonstersInRooms(map, rooms);
    // 몬스터 목록 가져오기
    monsters = SpawnManager::GetInstance()->GetActiveMonsters();

    render.DrawStaticUI();
    render.RenderHelp();

    if (stage < 1)
    {
        render.AddLog("게임이 시작되었습니다!", CLR_YELLOW);
        render.AddLog("WASD: 이동  |  `: 대쉬 활성화", CLR_GRAY);
    }

    render.AddLog(std::to_string(stage) + "층 입니다.", CLR_YELLOW);

    if (stage == 4)
        render.AddLog("보스룸 입니다.", CLR_YELLOW);


    running        = true;
    needsRedraw    = true;
    isBattleMode   = false;
    inputbutton    = false;
    isNextMoveDash = false;

    /*render.RenderMap(map, player, monsters);
    render.RenderInfo(player);
    render.RenderLog();*/
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
    Init(1);

    Render&       render = Render::GetInstance();
    InputManager& input  = InputManager::GetInstance();

    // 초기 화면 강제 렌더링
    render.RenderMap(map, player, monsters);
    render.RenderInfo(player);
    render.RenderLog();

    while (running)
    {
        // 입력 부하 방지 및 시스템 응답성 확보
        Sleep(5);

        GameAction action = input.PollInput();

        if (action != GameAction::None)
        {
            // 입력이 감지되면 즉시 처리 플래그 설정
            HandleAction(action);

            if (inputbutton)
            {
                UpdateMonster();
                inputbutton = false;
            }
            needsRedraw = true;
        }

        // 렌더링이 필요한 경우에만 수행 (needsRedraw)
        if (needsRedraw)
        {
            render.RenderMap(map, player, monsters);
            if (!inventoryOpen)
                render.RenderInfo(player);
            render.RenderLog();
            needsRedraw = false;
        }

        if (!player->IsAlive())
        {
            render.AddLog("더 이상 일어설 수 없습니다...", CLR_RED);
            render.RenderLog();
            Sleep(2000);
            running = false;
        }
    }

    system("cls");
    std::cout << "\n\n  GAME OVER...\n\n";
    Sleep(2000);
}

void GameManager::HandleAction(GameAction action)
{
    Render& render = Render::GetInstance();
    bool moved = false;
    //inputbutton = true;
    // 전투 중 명령
    if (isBattleMode && battleTarget != nullptr)
    {
        if (battleTarget->IsDead())
        {
            isBattleMode = false;
            battleTarget = nullptr;
            return;
        }

        switch (action)
        {
            case GameAction::Action1:
            player->Attack(battleTarget);
            if (battleTarget && !battleTarget->IsDead()) battleTarget->Attack(player);
            break;

            case GameAction::Action2:
            if (rand() % 100 < 55)
            {
                render.AddLog("성공적으로 도망쳤습니다!", CLR_CYAN);
                isBattleMode = false;
                battleTarget->SetState(MonsterState::IDLE);
                battleTarget = nullptr;
                player->SetX(ClampValue(player->GetX() + 1, 0, MAP_W - 1));
            }
            else
            {
                render.AddLog("도망에 실패하여 공격당했습니다!", CLR_RED);
                if (battleTarget && !battleTarget->IsDead()) battleTarget->Attack(player);
            }
            break;

            case GameAction::Quit: running = false; break;
            default: break;
        }

        needsRedraw = true;
        return;
    }

    // 인벤토리가 열려있으면 인벤토리 입력 처리
    if (inventoryOpen)
    {
        if (action == GameAction::Inventory)
            CloseInventory();
        else
            HandleInventoryAction(action);
        return;
    }

    // 탐색 중 이동 - 대쉬 시 2칸 이동
    int moveDist = isNextMoveDash ? 2 : 1;
    int dx = 0, dy = 0;

    switch (action)
    {
        case GameAction::Inventory:
            OpenInventory();
            return;

        case GameAction::Dash:
        if (player->CanDash())
        {
            isNextMoveDash = true;
            render.AddLog("대쉬 기운이 감돕니다!", CLR_YELLOW);
        }
        else render.AddLog("게이지가 부족합니다.", CLR_DARK_GRAY);
        break;

        case GameAction::MoveUp:
            {
                player->Move(action, map);  inputbutton = true; break;
            }
        case GameAction::MoveDown:
            {
                player->Move(action, map);  inputbutton = true; break;
            }
        case GameAction::MoveLeft:
            {
                player->Move(action, map);  inputbutton = true; break;
            }
        case GameAction::MoveRight:
            {
                player->Move(action, map);  inputbutton = true; break;
            }
        case GameAction::Help: render.AddLog("WASD: 이동 | `: 대쉬 | 1: 공격 | 2: 도망", CLR_CYAN); break;
        case GameAction::Quit: running = false; break;
        default: break;
    }

    // 이동 처리 (몬스터 충돌 감지 포함)
    if (dx != 0 || dy != 0)
    {
        for (int i = 0; i < moveDist; ++i)
        {
            int nextX = ClampValue(player->GetX() + dx, 0, MAP_W - 1);
            int nextY = ClampValue(player->GetY() + dy, 0, MAP_H - 1);

            // 이동 경로 상에 몬스터가 있는지 즉시 체크
            Monster* victim = nullptr;
            for (Monster* m : monsters)
            {
                if (m && !m->IsDead() && m->GetX() == nextX && m->GetY() == nextY)
                {
                    victim = m;
                    break;
                }
            }

            // 몬스터 발견 시 그 칸까지만 이동하고 루프 중단
            if (victim)
            {
                player->SetX(nextX);
                player->SetY(nextY);
                moved = true;
                break;
            }

            // 몬스터가 없으면 이동 후 계속 진행 (대쉬일 경우 2번째 칸까지)
            player->SetX(nextX);
            player->SetY(nextY);
            moved = true;
        }
    }

    if (moved)
    {
        if (isNextMoveDash)
        {
            player->UseDash();
            isNextMoveDash = false;
        }
        else
        {
            // 대쉬를 사용하지 않은 일반 이동 턴에만 게이지 충전
            player->UpdateDash();
        }

        inputbutton = true;

        for (Monster* m : monsters)
        {
            if (m && !m->IsDead()) m->Update(player->GetX(), player->GetY(), map);
        }

        ProcessBattle();

        SpawnManager::GetInstance()->UpdateCleanup(map);
        monsters = SpawnManager::GetInstance()->GetActiveMonsters();

        needsRedraw = true;
    }
}

void GameManager::UpdateMonster()
{
    //if (isBattleMode) return;
    for (Monster* m : monsters)
    {
        if (m && !m->IsDead()) m->UpdateAI(map);
    }
}

void GameManager::ProcessBattle()
{
    battle.CheckCombat(player, monsters);

    for (Monster* m : monsters)
    {
        if (!m || m->IsDead()) continue;
        if (m->GetX() == player->GetX() && m->GetY() == player->GetY())
        {
            if (!isBattleMode)
            {
                isBattleMode = true;
                battleTarget = m;
                isNextMoveDash = false;
                Render::GetInstance().AddLog(" [ 전투 시작 ] ", CLR_MAGENTA);
                Render::GetInstance().AddLog(" 1: 공격 | 2: 도망 ", CLR_YELLOW);
            }
            break;
        }
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

#include "GameManager.h"
#include "../BspManager/BspManager.h"
#include "../Render/Render.h"
#include "../Player/Player.h"
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "../Item/ItemManager.h"
#include "../SpawnManager/SpawnManager.h"
#include "../Monster/Boss.h"

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

    SceneChange(stage);

    // 예시: 랜덤 아이템 지급
    //player->GetInventory().AddItem(itemMgr.CreateItemByCode(105));

    render.DrawStaticUI();
    render.RenderHelp();

    if (stage <= 1)
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
            if (!ItemManager::GetInstance().IsOpen())
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
            inputbutton = true;
            break;

            case GameAction::Quit: running = false; break;
            default: break;
        }

        needsRedraw = true;
        return;
    }

    // 인벤토리가 열려있으면 인벤토리 입력 처리
    if (ItemManager::GetInstance().IsOpen())
    {
        if (action == GameAction::Inventory)
            ItemManager::GetInstance().CloseInventory(player);
        else
            ItemManager::GetInstance().HandleInventoryAction(action, player, isBattleMode);
        return;
    }

    // 탐색 중 이동
    if (action == GameAction::MoveUp || action == GameAction::MoveDown ||
        action == GameAction::MoveLeft || action == GameAction::MoveRight)
    {
        int moveDist = isNextMoveDash ? 2 : 1;
        for (int i = 0; i < moveDist; ++i)
        {
            if (player->Move(action, map))
            {
                moved = true;
            }
            else
            {
                break; // 벽이나 장애물에 막힘
            }
        }
    }
    else
    {
        switch (action)
        {
        case GameAction::Inventory:
            ItemManager::GetInstance().OpenInventory(player);
            return;

        case GameAction::Dash:
            if (player->CanDash())
            {
                isNextMoveDash = true;
                render.AddLog("대쉬 기운이 감돕니다!", CLR_YELLOW);
            }
            else render.AddLog("게이지가 부족합니다.", CLR_DARK_GRAY);
            break;

        case GameAction::Help: render.AddLog("WASD:이동 | `:대쉬 | 1:공격 | 2:도망", CLR_CYAN); break;
        case GameAction::Quit: running = false; break;
        default: break;
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

        player->TickInvisibility();

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

void GameManager::SceneChange(int stage)
{
    stage = 4;
    if (stage >= 4)
    {
        // 보스룸: 중앙 20x20 Floor, 나머지 Wall
        map.GenerateBossRoom();

        // 플레이어를 보스룸 중앙에 배치
        const int bossRoomCenterX = MAP_W / 2;
        const int bossRoomCenterY = MAP_H / 2;
        player->SetX(bossRoomCenterX);
        player->SetY(bossRoomCenterY);
        map.SetTile(bossRoomCenterX, bossRoomCenterY, Tile::Player);
    }
    else
    {
        // BSP 맵 생성
        BspManager::Params bspParams;
        BspManager::GetInstance().Generate(map, bspParams);

        const std::vector<Room>& rooms = BspManager::GetInstance().GetRooms();

        // 플레이어 스폰
        SpawnManager::GetInstance()->SpawnPlayerInRooms(map, rooms, player);

        // 오브젝트 스폰
        SpawnManager::GetInstance()->SpawnObjectInRooms(map, rooms);

        // 몬스터 스폰
        SpawnManager::GetInstance()->SpawnMonstersInRooms(map, rooms, stage);
        // 몬스터 목록 가져오기
        monsters = SpawnManager::GetInstance()->GetActiveMonsters();
    }
}

void GameManager::UpdateMonster()
{
    if (isBattleMode) return;
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

        int dist = std::max(std::abs(player->GetX() - m->GetX()), std::abs(player->GetY() - m->GetY()));
        if (dist <= 1)
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


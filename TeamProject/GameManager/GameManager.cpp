#include "GameManager.h"
#include "../Render/Render.h"
#include "../Player/Player.h"
#include <windows.h>
#include <cstdlib>
#include <ctime>
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
    
    
    
    // BSP 맵 생성
    BspManager::Params bspParams;
    // bspParams.seed     = 12345;  // 고정 시드가 필요하면 주석 해제
    // bspParams.maxDepth = 4;      // 깊이 줄이면 방이 크고 적어짐
    // bspParams.minPartitionW = 40; bspParams.minPartitionH = 40; // 큰 방
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
        render.AddLog("moved up",    CLR_DARK_GRAY);
        player->Move(action, map);
        inputbutton = true;
        break;
        
        case GameAction::MoveDown:
        render.AddLog("moved down",  CLR_DARK_GRAY);
        player->Move(action, map);
        inputbutton = true;
        break;
        
        case GameAction::MoveLeft:
        render.AddLog("moved left",  CLR_DARK_GRAY);
        player->Move(action, map);
        inputbutton = true;
        break;
        
        case GameAction::MoveRight:
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

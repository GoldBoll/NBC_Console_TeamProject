#include "GameManager.h"
#include "../Render/Render.h"
#include <windows.h>

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
    Render& render = Render::GetInstance();
    render.Init("TextRPG - NBC Team Project 2");

    

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


    render.DrawStaticUI();
    render.RenderHelp();

    render.AddLog("Game started!",              CLR_YELLOW);
    render.AddLog("WASD / arrow keys to move.", CLR_GRAY);

    running     = true;
    needsRedraw = true;
}

void GameManager::Run()
{
    Init();

    Render&       render = Render::GetInstance();
    InputManager& input  = InputManager::GetInstance();

    while (running)
    {
        if (needsRedraw)
        {
            render.RenderMap(map, player);
            render.RenderInfo(player);
            render.RenderLog();
            needsRedraw = false;
        }

        GameAction action = input.PollInput();
        HandleAction(action);
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
        player->SetY(player->GetY() - 1);
        needsRedraw = true;
        break;

    case GameAction::MoveDown:
        render.AddLog("moved down",  CLR_DARK_GRAY);
        player->SetY(player->GetY() + 1);
        needsRedraw = true;
        break;

    case GameAction::MoveLeft:
        render.AddLog("moved left",  CLR_DARK_GRAY);
        player->SetX(player->GetX() - 1);
        needsRedraw = true;
        break;

    case GameAction::MoveRight:
        render.AddLog("moved right", CLR_DARK_GRAY);
        player->SetX(player->GetX() + 1);
        needsRedraw = true;
        break;

    case GameAction::Help:
        render.AddLog("WASD/arrows: move  |  h: help  |  q: quit", CLR_CYAN);
        needsRedraw = true;
        break;

    case GameAction::Quit:
        running = false;
        break;

    default:
        break;
    }
}

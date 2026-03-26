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

    

    //player->SetJobName("Hero");
    //player->SetX(MAP_W / 2);
    //player.SetY(MAP_H / 2);

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

        Sleep(16);
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
        needsRedraw = true;
        break;

    case GameAction::MoveDown:
        render.AddLog("moved down",  CLR_DARK_GRAY);
        needsRedraw = true;
        break;

    case GameAction::MoveLeft:
        render.AddLog("moved left",  CLR_DARK_GRAY);
        needsRedraw = true;
        break;

    case GameAction::MoveRight:
        render.AddLog("moved right", CLR_DARK_GRAY);
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

#pragma once
#define NOMINMAX
#include <windows.h>
#include <string>
#include <deque>
#include <utility>
#include "../Map/Map.h"
// UILayout.h 는 Map.h → UILayout.h 경로로 이미 포함됩니다.

// 콘솔 색상 (Windows 16색)
enum ConsoleColor
{
    CLR_BLACK        = 0,
    CLR_DARK_BLUE    = 1,
    CLR_DARK_GREEN   = 2,
    CLR_DARK_CYAN    = 3,
    CLR_DARK_RED     = 4,
    CLR_DARK_MAGENTA = 5,
    CLR_DARK_YELLOW  = 6,
    CLR_GRAY         = 7,
    CLR_DARK_GRAY    = 8,
    CLR_BLUE         = 9,
    CLR_GREEN        = 10,
    CLR_CYAN         = 11,
    CLR_RED          = 12,
    CLR_MAGENTA      = 13,
    CLR_YELLOW       = 14,
    CLR_WHITE        = 15,
};
// 레이아웃 상수는 UILayout.h 를 참조하세요.

class Render
{
public:
    static Render& GetInstance();

    void Init(const char* title = "TextRPG");
    void DrawStaticUI();
    void RenderHelp();
    void RenderMap(const Map& map, const Player* player, const std::vector<Monster*>& monsters);
    void RenderInfo(Player* player);
    void RenderLog();
    void AddLog(const std::string& msg, int color = CLR_GREEN);

    Render(const Render&)            = delete;
    Render& operator=(const Render&) = delete;

private:
    Render();

    HANDLE hOut;
    std::deque<std::pair<std::string, int>> logLines;

    void GotoXY(int x, int y) const;
    void SetColor(int fg, int bg = CLR_BLACK) const;
    void ResetColor() const;
    void ClearRegion(int x, int y, int w, int h) const;
    void DrawBox(int x, int y, int w, int h,
                 const std::string& title = "",
                 int borderColor = CLR_DARK_CYAN) const;
};

// Render.cpp

#include "Render.h"
#include <iostream>
#include <string>
#include <algorithm>

Render& Render::GetInstance()
{
    static Render instance;
    return instance;
}

Render::Render()
    : hOut(GetStdHandle(STD_OUTPUT_HANDLE))
{}

void Render::Init(const char* title)
{
    // 1. 작업 표시줄을 제외한 실제 사용 가능한 화면 크기 조회
    RECT workArea = {};
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    int screenW = workArea.right  - workArea.left;
    int screenH = workArea.bottom - workArea.top;

    // 2. 기본 폰트 크기로 창을 키움
    constexpr SHORT TARGET_FONT = 16;
    SHORT fontByW  = (SHORT)(screenW / TOTAL_W);
    SHORT fontByH  = (SHORT)((screenH - 40) / TOTAL_H);
    SHORT fontSize = min(TARGET_FONT, min(fontByW, fontByH));
    fontSize = max((SHORT)6, fontSize);

    CONSOLE_FONT_INFOEX fontInfo = {};
    fontInfo.cbSize       = sizeof(fontInfo);
    fontInfo.dwFontSize   = { 0, fontSize };
    fontInfo.FontFamily   = FF_DONTCARE;
    fontInfo.FontWeight   = FW_NORMAL;
    wcscpy_s(fontInfo.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hOut, FALSE, &fontInfo);

    // 3. 창을 최소 크기로 줄여야 버퍼 축소/확대 시 충돌이 없음
    SMALL_RECT minRect = { 0, 0, 1, 1 };
    SetConsoleWindowInfo(hOut, TRUE, &minRect);

    // 4. 버퍼 크기 = 전체 콘솔 크기 (스크롤바 없음)
    COORD bufSize = { (SHORT)TOTAL_W, (SHORT)TOTAL_H };
    SetConsoleScreenBufferSize(hOut, bufSize);

    // 5. 창 크기를 버퍼와 동일하게 맞춤
    SMALL_RECT winRect = { 0, 0, (SHORT)(TOTAL_W - 1), (SHORT)(TOTAL_H - 1) };
    SetConsoleWindowInfo(hOut, TRUE, &winRect);

    CONSOLE_CURSOR_INFO ci = { 1, FALSE };
    SetConsoleCursorInfo(hOut, &ci);

    SetConsoleTitleA(title);
}

void Render::DrawStaticUI()
{
    system("cls");
    DrawBox(MAP_BOX_X,     MAP_BOX_Y,     MAP_BOX_W, MAP_BOX_H, "Map",         CLR_DARK_GREEN);
    DrawBox(PLAYER_INFO_X, PLAYER_INFO_Y, MID_BOX_W, PLAYER_INFO_H, "Player Info", CLR_DARK_MAGENTA);
    DrawBox(INFO2_BOX_X,   INFO2_BOX_Y,   MID_BOX_W, INFO2_H,   "Info 2",      CLR_DARK_RED);
    DrawBox(LOG_BOX_X,     LOG_BOX_Y,     LOG_BOX_W, LOG_BOX_H, "Log",         CLR_DARK_BLUE);
    DrawBox(HELP_BAR_X,    HELP_BAR_Y,    HELP_BAR_W, HELP_BAR_H, "",          CLR_DARK_YELLOW);
}

void Render::RenderHelp()
{
    struct Entry { const char* key; const char* desc; int color; };
    static const Entry entries[] = {
        { "WASD", "move",  CLR_YELLOW },
        { "h",    "help",  CLR_CYAN   },
        { "i",    "inven", CLR_GREEN  },
        { "q",    "quit",  CLR_RED    },
    };
    constexpr int entryCount = (int)(sizeof(entries) / sizeof(entries[0]));

    ClearRegion(HELP_BAR_X + 1, HELP_BAR_Y + 1, HELP_BAR_W - 2, HELP_BAR_H - 2);
    GotoXY(HELP_BAR_X + 1, HELP_BAR_Y + 1);

    for (int i = 0; i < entryCount; ++i)
    {
        SetColor(CLR_DARK_GRAY);
        std::cout << '[';
        SetColor(CLR_GRAY);
        std::cout << entries[i].desc << ':';
        SetColor(entries[i].color);
        std::cout << entries[i].key;
        SetColor(CLR_DARK_GRAY);
        std::cout << ']';
        SetColor(CLR_DARK_GRAY);
        std::cout << ' ';
    }
    ResetColor();
}

void Render::RenderMap(const Map& map, const Player* player)
{
    int camX = std::max(0, std::min(player->GetX() - VIEW_W / 2, MAP_W - VIEW_W));
    int camY = std::max(0, std::min(player->GetY() - VIEW_H / 2, MAP_H - VIEW_H));

    for (int row = 0; row < VIEW_H; ++row)
    {
        GotoXY(MAP_BOX_X + 1, MAP_BOX_Y + 1 + row);
        for (int col = 0; col < VIEW_W; ++col)
        {
            int wx = camX + col;
            int wy = camY + row;

            if (wx < 0 || wy < 0 || wx >= MAP_W || wy >= MAP_H)
            {
                SetColor(CLR_DARK_GRAY);
                std::cout << ' ';
                continue;
            }

            if (wx == player->GetX() && wy == player->GetY())
            {
                SetColor(CLR_WHITE, CLR_DARK_RED);
                std::cout << '@';
                continue;
            }

            Tile tile = map.GetTile(wx, wy);
            char glyph;
            switch (tile)
            {
            case Tile::Floor:
                SetColor(CLR_DARK_GRAY);   glyph = ' '; break;
            case Tile::Wall:
                SetColor(CLR_GRAY);        glyph = '#'; break;
            case Tile::Rock:
                SetColor(CLR_DARK_YELLOW); glyph = 'R'; break;
            case Tile::Water:
                SetColor(CLR_DARK_CYAN);   glyph = 'W'; break;
            case Tile::Stair:
                SetColor(CLR_YELLOW);      glyph = '>'; break;
            case Tile::Chest:
                SetColor(CLR_DARK_YELLOW); glyph = 'C'; break;
            case Tile::Monster:
                SetColor(CLR_RED);         glyph = 'M'; break;
            case Tile::EliteMonster:
                SetColor(CLR_MAGENTA);     glyph = 'E'; break;
            case Tile::Door:
                SetColor(CLR_DARK_YELLOW, CLR_DARK_RED); glyph = '+'; break;
            default:
                SetColor(CLR_DARK_GRAY);   glyph = ' '; break;
            }
            std::cout << glyph;
        }
    }
    ResetColor();
}

void Render::RenderInfo(Player* player)
{
    ClearRegion(PLAYER_INFO_X + 1, PLAYER_INFO_Y + 1, MID_BOX_W - 2, PLAYER_INFO_H - 2);

    int y = PLAYER_INFO_Y + 1;

    SetColor(CLR_WHITE);
    GotoXY(PLAYER_INFO_X + 1, y++);
    std::cout << (player->GetJobName().empty() ? "Unknown" : player->GetJobName());
    SetColor(CLR_DARK_CYAN);
    GotoXY(PLAYER_INFO_X + 1, y++);
    std::cout << "(" << player->GetX() << ", " << player->GetY() << ")";

    ++y;

    int barLen = MID_BOX_W - 6;
    int filled = (player->GetMaxHP() > 0)
        ? (int)((float)player->GetHP() / player->GetMaxHP() * barLen)
        : 0;
    filled = std::max(0, std::min(filled, barLen));

    SetColor(CLR_GRAY);
    GotoXY(PLAYER_INFO_X + 1, y);
    std::cout << "HP ";
    SetColor(CLR_RED);
    for (int i = 0; i < filled; ++i)        std::cout << '#';
    SetColor(CLR_DARK_GRAY);
    for (int i = filled; i < barLen; ++i)   std::cout << '-';

    SetColor(CLR_GRAY);
    GotoXY(PLAYER_INFO_X + 1, ++y);
    std::cout << player->GetHP() << "/" << player->GetMaxHP();

    ++y;

    SetColor(CLR_YELLOW);
    GotoXY(PLAYER_INFO_X + 1, ++y);
    std::cout << "Lv." << player->GetLevel()
              << "  EXP " << player->GetExp() << "/" << player->GetExpToNext();

    ResetColor();
}

void Render::RenderLog()
{
    ClearRegion(LOG_BOX_X + 1, LOG_BOX_Y + 1, LOG_BOX_W - 2, LOG_BOX_H - 2);

    int total     = (int)logLines.size();
    int startIdx  = std::max(0, total - LOG_MAX_LINES);
    int drawCount = total - startIdx;

    for (int i = 0; i < drawCount; ++i)
    {
        const std::string& msg   = logLines[startIdx + i].first;
        const int          color = logLines[startIdx + i].second;

        GotoXY(LOG_BOX_X + 1, LOG_BOX_Y + 1 + i);
        SetColor(color);

        std::string line = msg.substr(0, (size_t)(LOG_BOX_W - 2));
        std::cout << line;
    }
    ResetColor();
}

void Render::AddLog(const std::string& msg, int color)
{
    logLines.push_back({ msg, color });
    if ((int)logLines.size() > LOG_MAX_LINES)
        logLines.pop_front();
}

void Render::GotoXY(int x, int y) const
{
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, pos);
}

void Render::SetColor(int fg, int bg) const
{
    SetConsoleTextAttribute(hOut, (WORD)((bg << 4) | fg));
}

void Render::ResetColor() const
{
    SetColor(CLR_GRAY, CLR_BLACK);
}

void Render::ClearRegion(int x, int y, int w, int h) const
{
    ResetColor();
    std::string blank((size_t)w, ' ');
    for (int i = 0; i < h; ++i)
    {
        GotoXY(x, y + i);
        std::cout << blank;
    }
}

void Render::DrawBox(int x, int y, int w, int h,
                     const std::string& title,
                     int borderColor) const
{
    SetColor(borderColor);

    GotoXY(x, y);
    std::cout << '+';
    if (!title.empty())
    {
        std::string t = '-' + title + '-';
        std::cout << t;
        int remaining = w - 2 - (int)t.size();
        for (int i = 0; i < remaining; ++i) std::cout << '-';
    }
    else
    {
        for (int i = 0; i < w - 2; ++i) std::cout << '-';
    }
    std::cout << '+';

    for (int i = 1; i < h - 1; ++i)
    {
        GotoXY(x,         y + i); std::cout << '|';
        GotoXY(x + w - 1, y + i); std::cout << '|';
    }

    GotoXY(x, y + h - 1);
    std::cout << '+';
    for (int i = 0; i < w - 2; ++i) std::cout << '-';
    std::cout << '+';

    ResetColor();
}

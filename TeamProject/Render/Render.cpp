// Render.cpp

#include "Render.h"
#include <iostream>
#include <string>
#include "../Monster/Monster.h"
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

void Render::RenderMap(const Map& map, const Player* player, const std::vector<Monster*>& monsters)
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

            Tile tile = map.GetTile(wx, wy);
            char glyph;
            switch (tile)
            {
                case Tile::Floor:
                SetColor(CLR_DARK_GRAY);   glyph = ' '; break;
                case Tile::Wall:
                if (map.HasFloorNeighbor(wx, wy))
                {
                    if (map.GetDebugRoomWall(wx, wy))
                    SetColor(CLR_CYAN);   // 디버그: 방 경계 벽 (하늘색)
                    else
                    SetColor(CLR_GRAY);   // 복도 벽 (회색)
                    glyph = '#';
                }
                else
                { SetColor(CLR_DARK_GRAY); glyph = ' '; }
                break;
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
                default:
                SetColor(CLR_DARK_GRAY);   glyph = ' '; break;
            }
            std::cout << glyph;
        }
    }

    // ── 몬스터 객체 렌더링 ───────────────────────────────────────────────────
    for (Monster* monster : monsters)
    {
        if (monster == nullptr || monster->IsDead()) continue;

        int mx = monster->GetX();
        int my = monster->GetY();

        // 현재 카메라 화면(Viewport) 내부에 있는지 확인
        if (mx >= camX && mx < camX + VIEW_W &&
            my >= camY && my < camY + VIEW_H)
            {
                GotoXY(MAP_BOX_X + 1 + (mx - camX),
                MAP_BOX_Y + 1 + (my - camY));

                Tile currentTile = monster->GetDisplayTile();
                char glyph;
                int color;

                
                if(currentTile == Tile::Chest) 
                {
                    // 미믹일 경우
                    glyph = 'C';
                    color = CLR_DARK_YELLOW;
                }
                else
                {
                    glyph = monster->IsElite() ? 'E' : 'M';
                    color = monster->IsElite() ? CLR_MAGENTA : CLR_RED;
                }

                if (monster->GetState() == MonsterState::CHASE)
                {
                    SetColor(color, CLR_DARK_YELLOW);
                }
                else if (monster->GetState() == MonsterState::COMBAT)
                {
                    SetColor(CLR_WHITE, CLR_RED); // 전투 중이면 반전
                }
                else
                {
                    SetColor(color);
                }
                std::cout << glyph;
            }
        }

        // ── 플레이어 렌더링 (가장 위에 그려지도록 마지막에 이동) ───────────────
        if (player->GetX() >= camX && player->GetX() < camX + VIEW_W &&
        player->GetY() >= camY && player->GetY() < camY + VIEW_H)
        {
            GotoXY(MAP_BOX_X + 1 + (player->GetX() - camX),
            MAP_BOX_Y + 1 + (player->GetY() - camY));
            SetColor(CLR_WHITE, CLR_DARK_RED);
            std::cout << '@';
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

        // --- DASH Bar 추가 ---
        y += 2;
        int dashFilled = (player->GetDashGauge() * barLen) / 120; // maxDashGauge 120 기준
        dashFilled = std::max(0, std::min(dashFilled, barLen));

        SetColor(CLR_GRAY);
        GotoXY(PLAYER_INFO_X + 1, y);
        std::cout << "DS  ";
        SetColor(CLR_YELLOW);
        for (int i = 0; i < dashFilled; i++)        std::cout << '=';
        SetColor(CLR_DARK_GRAY);
        for (int i = dashFilled; i < barLen; i++)    std::cout << '-';

        SetColor(CLR_GRAY);
        GotoXY(PLAYER_INFO_X + 1, ++y);
        std::cout << player->GetDashGauge() << "/120";

        // ---------------------
        y += 1;
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

            size_t maxLen = (size_t)(LOG_BOX_W - 3);
            std::string line = msg;

            if (line.length() > maxLen)
            {
                line = msg.substr(0, maxLen);

                int isLead = 0;
                for (size_t k = 0; k < line.length(); ++k)
                {
                    if (IsDBCSLeadByte((BYTE)line[k]))
                    {
                        if (k == line.length() - 1) isLead = 1;
                        k++;
                    }
                }
                if (isLead && !line.empty()) line.pop_back();
            }

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

void Render::RenderInventory(const Inventory& inv, int selectedIdx, int scrollOffset)
{
    ClearRegion(PLAYER_INFO_X + 1, PLAYER_INFO_Y + 1, MID_BOX_W - 2, PLAYER_INFO_H - 2);

    const int maxVisible = PLAYER_INFO_H - 4;
    int y = PLAYER_INFO_Y + 1;

    SetColor(CLR_YELLOW);
    GotoXY(PLAYER_INFO_X + 1, y++);
    std::cout << "[ Inventory ]";
    ++y;

    auto items = inv.GetFilledItems();
    if (items.empty())
    {
        SetColor(CLR_DARK_GRAY);
        GotoXY(PLAYER_INFO_X + 1, y);
        std::cout << "(empty)";
        ResetColor();
        return;
    }

    for (int i = 0; i < maxVisible; ++i)
    {
        int listIdx = scrollOffset + i;
        if (listIdx >= (int)items.size()) break;

        GotoXY(PLAYER_INFO_X + 1, y + i);
        bool selected = (listIdx == selectedIdx);

        if (selected) { SetColor(CLR_WHITE);     std::cout << '>'; SetColor(CLR_YELLOW); }
        else          { SetColor(CLR_DARK_GRAY); std::cout << ' '; SetColor(CLR_GRAY);   }

        std::string name = items[listIdx].second->GetName();
        int cnt = items[listIdx].second->count;
        if (cnt > 1) name += " (" + std::to_string(cnt) + ")";
        int maxLen = MID_BOX_W - 4;
        if ((int)name.size() > maxLen) name = name.substr(0, maxLen);
        std::cout << name;
    }

    // 스크롤 표시 (아이템이 더 있을 때)
    if (scrollOffset + maxVisible < (int)items.size())
    {
        SetColor(CLR_DARK_GRAY);
        GotoXY(PLAYER_INFO_X + 1, PLAYER_INFO_Y + PLAYER_INFO_H - 2);
        std::cout << "v more";
    }

    ResetColor();
}

void Render::RenderItemDesc(const Item* item)
{
    ClearRegion(INFO2_BOX_X + 1, INFO2_BOX_Y + 1, MID_BOX_W - 2, INFO2_H - 2);

    if (!item) { ResetColor(); return; }

    int y = INFO2_BOX_Y + 1;
    const int maxW = MID_BOX_W - 2;

    SetColor(CLR_WHITE);
    GotoXY(INFO2_BOX_X + 1, y++);
    std::string name = item->GetName();
    if ((int)name.size() > maxW) name = name.substr(0, maxW);
    std::cout << name;
    ++y;

    SetColor(CLR_CYAN);
    GotoXY(INFO2_BOX_X + 1, y++);
    switch (item->type)
    {
    case ItemType::POTION: std::cout << "[POTION]"; break;
    default:               std::cout << "[ETC]";    break;
    }
    ++y;

    SetColor(CLR_GRAY);
    const std::string& desc = item->GetDescription();
    int start = 0;
    int descLen = (int)desc.size();
    while (start < descLen && y < INFO2_BOX_Y + INFO2_H - 1)
    {
        GotoXY(INFO2_BOX_X + 1, y++);
        std::cout << desc.substr(start, maxW);
        start += maxW;
    }

    ResetColor();
}

void Render::ClearInfo2()
{
    ClearRegion(INFO2_BOX_X + 1, INFO2_BOX_Y + 1, MID_BOX_W - 2, INFO2_H - 2);
}

void Render::ShowReviveToken()
{
    ClearRegion(INFO2_BOX_X + 1, INFO2_BOX_Y + 1, MID_BOX_W - 2, INFO2_H - 2);

    int y = INFO2_BOX_Y + 1;
    SetColor(CLR_YELLOW);
    GotoXY(INFO2_BOX_X + 1, y++);
    std::cout << "[ 부활 토큰 활성화 ]";
    ++y;

    ResetColor();
}

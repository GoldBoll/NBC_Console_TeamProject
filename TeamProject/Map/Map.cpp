#include "Map.h"
#include "../Player/Player.h"
#include <algorithm>
using namespace std;

bool Map::HasFloorNeighbor(int x, int y) const
{
    for (int dy = -1; dy <= 1; ++dy)
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx, ny = y + dy;
            if (InBounds(nx, ny) && tiles[ny][nx] == Tile::Floor)
                return true;
        }
    return false;
}

char Map::TileToChar(int x, int y) const
{
    switch (tiles[y][x])
    {
        case Tile::Floor:        return ' ';  // 빈 공간 (투명)
        case Tile::Wall:         return '#';  // 벽
        case Tile::Rock:         return 'R';  // 바위
        case Tile::Water:        return 'W';  // 물
        case Tile::Stair:        return '>';  // 계단
        case Tile::Chest:        return 'C';  // 보물 상자
        case Tile::Monster:      return 'M';  // 일반 몬스터
        case Tile::EliteMonster: return 'E';  // 엘리트 몬스터
        case Tile::Boss:         return 'B';  // 보스
        case Tile::Door:         return '+';  // 문
        default:                 return ' ';
    }
}

// void Map::LoadStage1()
// {
//     // ── 1. 전체 Floor 로 초기화 ──────────────────────────────────────────────
//     for (int y = 0; y < MAP_H; ++y)
//         for (int x = 0; x < MAP_W; ++x)
//             tiles[y][x] = Tile::Floor;
//
//     // ── 2. 외벽 (Wall) ───────────────────────────────────────────────────────
//     for (int x = 0; x < MAP_W; ++x)
//     {
//         tiles[0][x]         = Tile::Wall;
//         tiles[MAP_H - 1][x] = Tile::Wall;
//     }
//     for (int y = 0; y < MAP_H; ++y)
//     {
//         tiles[y][0]         = Tile::Wall;
//         tiles[y][MAP_W - 1] = Tile::Wall;
//     }
//
//     // ── 3. 바위 지형 (Rock) ──────────────────────────────────────────────────
//     for (int y = 10; y < 18; ++y)
//         for (int x = 10; x < 16; ++x)
//             tiles[y][x] = Tile::Rock;
//
//     for (int y = MAP_H - 30; y < MAP_H - 22; ++y)
//         for (int x = MAP_W - 20; x < MAP_W - 12; ++x)
//             tiles[y][x] = Tile::Rock;
//
//     // ── 4. 물 지형 (Water) ───────────────────────────────────────────────────
//     for (int y = STAGE1_START_Y - 40; y < STAGE1_START_Y - 20; ++y)
//         for (int x = STAGE1_START_X - 15; x < STAGE1_START_X + 15; ++x)
//             tiles[y][x] = Tile::Water;
//
//     for (int y = 20; y < MAP_H - 20; ++y)
//         for (int x = MAP_W - 60; x < MAP_W - 55; ++x)
//             tiles[y][x] = Tile::Water;
//
//     // ── 5. 보물 상자 (Chest) ─────────────────────────────────────────────────
//     tiles[STAGE1_START_Y - 10][STAGE1_START_X + 20] = Tile::Chest;
//     tiles[STAGE1_START_Y + 15][STAGE1_START_X - 30] = Tile::Chest;
//     tiles[30][30]                                    = Tile::Chest;
//
//     // ── 6. 일반 몬스터 (Monster) ─────────────────────────────────────────────
//     tiles[STAGE1_START_Y - 5][STAGE1_START_X + 5]  = Tile::Monster;
//     tiles[STAGE1_START_Y + 8][STAGE1_START_X - 8]  = Tile::Monster;
//     tiles[STAGE1_START_Y][STAGE1_START_X + 15]      = Tile::Monster;
//     tiles[20][50]                                    = Tile::Monster;
//     tiles[80][40]                                    = Tile::Monster;
//
//     // ── 7. 엘리트 몬스터 (EliteMonster) ─────────────────────────────────────
//     tiles[STAGE1_START_Y + 30][STAGE1_START_X + 30] = Tile::EliteMonster;
//     tiles[MAP_H - 10][10]                            = Tile::EliteMonster;
//
//     // ── 8. 계단 (Stair) - 다음 스테이지 진입점 ──────────────────────────────
//     tiles[MAP_H - 2][MAP_W - 2] = Tile::Stair;
// }

// void Map::DrawViewport(const Player& player) const
// {
//     int camX = max(0, min(player.GetX() - VIEW_W / 2, MAP_W - VIEW_W));
//     int camY = max(0, min(player.GetY() - VIEW_H / 2, MAP_H - VIEW_H));
//
//     for (int row = 0; row < VIEW_H; row++)
//     {
//         for (int col = 0; col < VIEW_W; col++)
//         {
//             int mx = camX + col;
//             int my = camY + row;
//             cout << TileToChar(mx, my);
//         }
//         cout << '\n';
//     }
// }

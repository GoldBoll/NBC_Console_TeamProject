#include "Map.h"
#include "../Player/Player.h"
#include <algorithm>
using namespace std;

// ── Map 멤버 함수 ─────────────────────────────────────

bool Map::isWalkable(int x, int y) const
{
    return tiles[y][x] == Tile::Floor
        || tiles[y][x] == Tile::Stair;
}

char Map::tileToChar(int x, int y) const
{
    switch (tiles[y][x])
    {
    case Tile::Floor:    return '.';
    case Tile::Wall:     return '#';
    case Tile::Obstacle: return 'W';
    case Tile::Stair:    return '>';
    default:             return ' ';
    }
}


void drawViewport(const Map& map, const Player& player)
{
    // 카메라 좌상단 좌표 (플레이어 중심)
    int camX = player.x - VIEW_W / 2;
    int camY = player.y - VIEW_H / 2;

    // 맵 경계를 벗어나지 않도록 클램프
    camX = max(0, min(camX, MAP_W - VIEW_W));
    camY = max(0, min(camY, MAP_H - VIEW_H));

    for (int row = 0; row < VIEW_H; row++)
    {
        for (int col = 0; col < VIEW_W; col++)
        {
            int mx = camX + col;
            int my = camY + row;
            cout << map.tileToChar(mx, my);
        }
        cout << '\n';
    }
}

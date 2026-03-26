#include "Map.h"
#include "../Player/Player.h"
#include <algorithm>
using namespace std;

bool Map::IsWalkable(int x, int y) const
{
    return tiles[y][x] == Tile::Floor
        || tiles[y][x] == Tile::Stair;
}

char Map::TileToChar(int x, int y) const
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

void DrawViewport(const Map& map, const Player& player)
{
    int camX = max(0, min(player.x - VIEW_W / 2, MAP_W - VIEW_W));
    int camY = max(0, min(player.y - VIEW_H / 2, MAP_H - VIEW_H));

    for (int row = 0; row < VIEW_H; row++)
    {
        for (int col = 0; col < VIEW_W; col++)
        {
            int mx = camX + col;
            int my = camY + row;
            cout << map.TileToChar(mx, my);
        }
        cout << '\n';
    }
}

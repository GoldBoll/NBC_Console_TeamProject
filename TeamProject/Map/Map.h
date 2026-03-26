#pragma once
#include <iostream>
using namespace std;
#include "../Render/UILayout.h"
#include "../Player/Player.h"

// 맵 상수
constexpr int MAP_W = 500;
constexpr int MAP_H = 500;
// VIEW_W, VIEW_H 는 UILayout.h 에서 Map 비율에 따라 자동 계산됩니다.

// 타일 타입
enum class Tile
{
    Floor,
    Wall,
    Obstacle,
    Stair,
};

// 맵 데이터
class Map
{
public:
    Tile tiles[MAP_H][MAP_W];

    bool IsWalkable(int x, int y) const;
    char TileToChar(int x, int y) const;
};

void DrawViewport(const Map& map, const Player& player);

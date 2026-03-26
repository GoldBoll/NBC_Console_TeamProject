#pragma once
#include <iostream>
using namespace std;
#include "../Player/Player.h"

// ── 맵 상수 ──────────────────────────────────────────
constexpr int MAP_W  = 500;
constexpr int MAP_H  = 500;
constexpr int VIEW_W = 40;   // 콘솔 뷰포트 가로
constexpr int VIEW_H = 20;   // 콘솔 뷰포트 세로

// ── 타일 타입 ─────────────────────────────────────────
enum class Tile
{
    Floor,      // '.'  이동 가능
    Wall,       // '#'  이동 불가
    Obstacle,   // 'W'  구조물
    Stair,      // '>'  탈출 계단
};

// ── 맵 데이터 ─────────────────────────────────────────
struct Map
{
    Tile tiles[MAP_H][MAP_W];

    bool isWalkable(int x, int y) const;
    char tileToChar(int x, int y) const;
};

// ── 뷰포트 렌더 ───────────────────────────────────────
struct Player;  // 전방 선언

void drawViewport(const Map& map, const Player& player);

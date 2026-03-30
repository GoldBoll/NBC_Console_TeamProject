#pragma once
#include <iostream>
#include <algorithm>
using namespace std;
#include "../Render/UILayout.h"
#include "../Player/Player.h"

// 맵 상수
constexpr int MAP_W = 100;
constexpr int MAP_H = 100;

// VIEW_W, VIEW_H 는 UILayout.h 에서 Map 비율에 따라 자동 계산됩니다.
constexpr int STAGE1_START_X = MAP_W / 2;
constexpr int STAGE1_START_Y = MAP_H / 2;

// BSP 로 생성된 방 정보 (BspManager 가 채워주고, SpawnManager/GameManager 가 읽음)
struct Room
{
    int x, y, w, h;                          // Floor 영역의 좌상단 좌표 및 크기
    int CenterX() const { return x + w / 2; }
    int CenterY() const { return y + h / 2; }
};


// 타일 타입
enum class Tile
{
    Floor,          // ' '  빈 공간 (투명 처리, 이동 가능)
    Wall,           // '#'  벽 (이동 불가)
    Rock,           // 'R'  바위 - 좁은 장애물 (이동 불가)
    Water,          // 'W'  물 - 넓은 장애물 (이동 불가)
    Stair,          // '>'  계단 - 다음 층으로 이동
    Chest,          // 'C'  보물 상자 - 아이템 획득
    Monster,        // 'M'  일반 몬스터
    EliteMonster,   // 'E'  엘리트 몬스터 - 강화된 몬스터
    Door,           // '+'  문 - 방과 복도 사이 통로 (이동 가능)
};

// 맵 데이터
class Map
{
protected:
    Tile tiles[MAP_H][MAP_W];

    // 디버그: BSP 방 경계 벽 여부 (true = 방 벽, false = 복도 벽 또는 일반 벽)
    bool debugRoomWall[MAP_H][MAP_W] = {};

public:
    // 타일 읽기
    Tile GetTile(int x, int y)              const { return tiles[y][x]; }

    // 타일 쓰기
    void SetTile(int x, int y, Tile tile)         { tiles[y][x] = tile; }

    // 범위 내 좌표인지 확인
    bool InBounds(int x, int y)             const { return x >= 0 && x < MAP_W && y >= 0 && y < MAP_H; }

    // 맵 전체를 특정 타일로 채움
    void Fill(Tile t) { std::fill(&tiles[0][0], &tiles[0][0] + MAP_W * MAP_H, t); }

    // 디버그 방 경계 벽 접근
    bool GetDebugRoomWall(int x, int y)     const { return debugRoomWall[y][x]; }
    void SetDebugRoomWall(int x, int y, bool v)   { debugRoomWall[y][x] = v; }
    void ClearDebugRoomWall() { std::fill(&debugRoomWall[0][0], &debugRoomWall[0][0] + MAP_W * MAP_H, false); }

    // 주변 8방향에 Floor 타일이 하나라도 있는지 확인 (렌더링에서 벽 표시 여부 판단용)
    bool HasFloorNeighbor(int x, int y) const;

    char TileToChar(int x, int y)           const;

    //레거시 코드
    //void LoadStage1();
    //void DrawViewport(const Player& player) const;
};


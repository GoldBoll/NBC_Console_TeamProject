#include "BspManager.h"
#include <ctime>
#include <algorithm>
#include <numeric>

BspManager& BspManager::GetInstance()
{
    static BspManager instance;
    return instance;
}

void BspManager::Generate(Map& map, const Params& p)
{
    curParams = p;

    unsigned seed = p.seed ? p.seed : (unsigned)time(nullptr);
    rng.seed(seed);

    // 기존 트리 해제 및 방 목록 초기화
    FreeTree(root);
    root = nullptr;
    rooms.clear();

    // 전체 Wall 로 초기화 (BSP 는 Wall 에서 깎아내는 방식)
    map.Fill(Tile::Wall);

    // 트리 빌드 → 방 조각
    root = BuildTree(0, 0, MAP_W, MAP_H, 0);
    CarveRooms(root, map);

    // 방 타일 스냅샷: 복도 팽창 시 방과 복도를 구분하기 위해 저장
    bool roomSnapshot[MAP_H][MAP_W] = {};
    BuildRoomSnapshot(map, roomSnapshot);

    // 1칸 너비 복도 중심선 연결 (Z자형)
    ConnectTree(root, map);

    // 복도 중심선을 4방향 팽창 → 3칸 너비로 확장
    DilateCorridor(map, roomSnapshot);

    // 디버그: 방 경계 벽 마킹 (복도 벽과 색상 구분용)
    MarkDebugRoomWalls(map);

    // 방 타입 랜덤 배정
    AssignRoomTypes(p);
}

//  BuildTree : 파티션을 재귀 분할해 BSP 트리 생성
BspManager::BspNode* BspManager::BuildTree(int x, int y, int w, int h, int depth)
{
    BspNode* node = new BspNode{ x, y, w, h };

    bool canSplitW = (w >= curParams.minPartitionW * 2);  // 좌우 분할 가능 여부
    bool canSplitH = (h >= curParams.minPartitionH * 2);  // 상하 분할 가능 여부

    if (depth >= curParams.maxDepth || (!canSplitW && !canSplitH))
        return node;   // leaf

    // 둘 다 가능하면 랜덤, 하나만 가능하면 그쪽으로
    bool splitVertical;
    if (canSplitW && canSplitH)
        splitVertical = (rng() % 2) == 0;
    else
        splitVertical = canSplitW;

    node->splitVertical = splitVertical;  // ConnectTree 에서 분할 방향 참조

    if (splitVertical)
    {
        // 세로 분리선: 좌 파티션 / 우 파티션
        int range = w - curParams.minPartitionW * 2;
        int split = curParams.minPartitionW
                  + (range > 0 ? (int)(rng() % (unsigned)range) : 0);

        node->left  = BuildTree(x,         y, split,     h, depth + 1);
        node->right = BuildTree(x + split, y, w - split, h, depth + 1);
    }
    else
    {
        // 가로 분리선: 상 파티션 / 하 파티션
        int range = h - curParams.minPartitionH * 2;
        int split = curParams.minPartitionH
                  + (range > 0 ? (int)(rng() % (unsigned)range) : 0);

        node->left  = BuildTree(x, y,         w, split,     depth + 1);
        node->right = BuildTree(x, y + split, w, h - split, depth + 1);
    }

    return node;
}

//  CarveRooms : leaf 노드마다 파티션 안에 랜덤 크기의 방을 Floor 로 조각
void BspManager::CarveRooms(BspNode* node, Map& map)
{
    if (!node) return;

    if (node->IsLeaf())
    {
        constexpr int MARGIN = 1;   // 파티션 경계와 방 사이 최소 여백 (벽 1칸 보장)

        int availW = node->w - MARGIN * 2;
        int availH = node->h - MARGIN * 2;

        // 방 크기: [minRoom, min(maxRoom, avail)] 범위에서 랜덤
        int maxW = std::min(availW, curParams.maxRoomW);
        int maxH = std::min(availH, curParams.maxRoomH);
        int minW = std::min(curParams.minRoomW, maxW);
        int minH = std::min(curParams.minRoomH, maxH);

        int rw = minW + (maxW > minW ? (int)(rng() % (unsigned)(maxW - minW + 1)) : 0);
        int rh = minH + (maxH > minH ? (int)(rng() % (unsigned)(maxH - minH + 1)) : 0);

        // 방 위치: 여백 안에서 랜덤 오프셋
        int spaceW = availW - rw;
        int spaceH = availH - rh;
        int ox = MARGIN + (spaceW > 0 ? (int)(rng() % (unsigned)spaceW) : 0);
        int oy = MARGIN + (spaceH > 0 ? (int)(rng() % (unsigned)spaceH) : 0);

        int rx = node->x + ox;
        int ry = node->y + oy;

        node->room = { rx, ry, rw, rh };
        rooms.push_back(node->room);

        for (int cy = ry; cy < ry + rh; ++cy)
            for (int cx = rx; cx < rx + rw; ++cx)
                if (map.InBounds(cx, cy))
                    map.SetTile(cx, cy, Tile::Floor);

        return;
    }

    CarveRooms(node->left,  map);
    CarveRooms(node->right, map);
}

//  ConnectTree : 파티션 경계(spine)를 통해 Z자형 복도 중심선(1칸 너비)을 연결
BspManager::ConnectPoint BspManager::ConnectTree(BspNode* node, Map& map)
{
    if (!node)          return {};
    if (node->IsLeaf()) return { node->room.CenterX(), node->room.CenterY() };

    ConnectPoint leftJunction  = ConnectTree(node->left,  map);
    ConnectPoint rightJunction = ConnectTree(node->right, map);

    if (node->splitVertical)
    {
        // 수직 분할(좌|우): H → V → H  Z자형
        const int spineCol = node->right->x;

        HCorridor(map, leftJunction.x,  spineCol,        leftJunction.y);
        VCorridor(map, spineCol,         leftJunction.y,  rightJunction.y);
        HCorridor(map, spineCol,         rightJunction.x, rightJunction.y);

        node->junctionX   = spineCol;
        node->junctionY   = leftJunction.y;
        node->hasJunction = true;

        return { spineCol, leftJunction.y };
    }
    else
    {
        // 수평 분할(상|하): V → H → V  역Z자형
        const int spineRow = node->right->y;

        VCorridor(map, leftJunction.x,  leftJunction.y,  spineRow);
        HCorridor(map, leftJunction.x,  rightJunction.x, spineRow);
        VCorridor(map, rightJunction.x, spineRow,         rightJunction.y);

        node->junctionX   = leftJunction.x;
        node->junctionY   = spineRow;
        node->hasJunction = true;

        return { leftJunction.x, spineRow };
    }
}

//  HCorridor : 지정한 행(row)에서 startCol~endCol 구간을 1칸 너비 중심선으로 연결
void BspManager::HCorridor(Map& map, int startCol, int endCol, int row)
{
    int leftCol  = std::min(startCol, endCol);
    int rightCol = std::max(startCol, endCol);
    for (int col = leftCol; col <= rightCol; ++col)
        if (map.InBounds(col, row))
            map.SetTile(col, row, Tile::Floor);
}

//  VCorridor : 지정한 열(col)에서 startRow~endRow 구간을 1칸 너비 중심선으로 연결
void BspManager::VCorridor(Map& map, int col, int startRow, int endRow)
{
    int topRow    = std::min(startRow, endRow);
    int bottomRow = std::max(startRow, endRow);
    for (int row = topRow; row <= bottomRow; ++row)
        if (map.InBounds(col, row))
            map.SetTile(col, row, Tile::Floor);
}

void BspManager::BuildRoomSnapshot(const Map& map, bool out[][MAP_W])
{
    for (int y = 0; y < MAP_H; ++y)
        for (int x = 0; x < MAP_W; ++x)
            out[y][x] = (map.GetTile(x, y) == Tile::Floor);
}

//  DilateCorridor : 복도 중심선(1칸)을 4방향으로 1칸씩 팽창 → 총 3칸 너비로 확장
void BspManager::DilateCorridor(Map& map, const bool roomSnapshot[][MAP_W])
{
    constexpr int NEIGHBOR_DX[] = { -1, 1,  0, 0 };
    constexpr int NEIGHBOR_DY[] = {  0, 0, -1, 1 };

    // 팽창할 좌표를 수집 (순회 중 맵을 수정하면 결과가 달라지므로 분리)
    bool toExpand[MAP_H][MAP_W] = {};

    for (int y = 0; y < MAP_H; ++y)
    {
        for (int x = 0; x < MAP_W; ++x)
        {
            // 복도 중심선 타일: Floor 이면서 방이 아닌 것
            const bool isCorridorCenter =
                (map.GetTile(x, y) == Tile::Floor) && !roomSnapshot[y][x];

            if (isCorridorCenter)
            {
                for (int d = 0; d < 4; ++d)
                {
                    int nx = x + NEIGHBOR_DX[d];
                    int ny = y + NEIGHBOR_DY[d];
                    if (map.InBounds(nx, ny))
                        toExpand[ny][nx] = true;
                }
            }
        }
    }

    // 수집된 위치 중 Wall 타일만 Floor 로 변환
    for (int y = 0; y < MAP_H; ++y)
    {
        for (int x = 0; x < MAP_W; ++x)
        {
            if (toExpand[y][x] && map.GetTile(x, y) == Tile::Wall)
            {
                map.SetTile(x, y, Tile::Floor);
            }
        }
    }
}

void BspManager::MarkDebugRoomWalls(Map& map)
{
    map.ClearDebugRoomWall();
    for (const Room& room : rooms)
    {
        // 상단/하단 행 전체 (모서리 포함)
        for (int x = room.x - 1; x <= room.x + room.w; ++x)
        {
            const int topRow    = room.y - 1;
            const int bottomRow = room.y + room.h;
            if (map.InBounds(x, topRow)    && map.GetTile(x, topRow)    == Tile::Wall)
                map.SetDebugRoomWall(x, topRow, true);
            if (map.InBounds(x, bottomRow) && map.GetTile(x, bottomRow) == Tile::Wall)
                map.SetDebugRoomWall(x, bottomRow, true);
        }
        // 좌측/우측 열 (상하 모서리 제외 — 위에서 이미 처리)
        for (int y = room.y; y < room.y + room.h; ++y)
        {
            const int leftCol  = room.x - 1;
            const int rightCol = room.x + room.w;
            if (map.InBounds(leftCol,  y) && map.GetTile(leftCol,  y) == Tile::Wall)
                map.SetDebugRoomWall(leftCol, y, true);
            if (map.InBounds(rightCol, y) && map.GetTile(rightCol, y) == Tile::Wall)
                map.SetDebugRoomWall(rightCol, y, true);
        }
    }
}

// AssignRoomTypes : rooms[0] = Start, rooms.back() = Stair,
//                   나머지를 셔플 후 Boss → Elite → Treasure → Normal 순으로 배정
void BspManager::AssignRoomTypes(const Params& p)
{
    if (rooms.empty()) return;

    rooms.front().type = RoomType::Start;
    if (rooms.size() == 1) return;
    rooms.back().type = RoomType::Stair;

    // 중간 방 인덱스 풀 수집 후 셔플
    std::vector<int> pool;
    pool.resize(rooms.size() - 2);
    std::iota(pool.begin(), pool.end(), 1);   // 1 ~ rooms.size()-2
    std::shuffle(pool.begin(), pool.end(), rng);

    int idx = 0;
    auto assign = [&](RoomType t, int count)
    {
        for (int i = 0; i < count && idx < (int)pool.size(); ++i, ++idx)
            rooms[pool[idx]].type = t;
    };

    assign(RoomType::Boss,     p.bossRoomCount);
    assign(RoomType::Elite,    p.eliteRoomCount);
    assign(RoomType::Treasure, p.treasureRoomCount);
    // 나머지는 생성 시 기본값인 Normal 유지
}

void BspManager::FreeTree(BspNode* node)
{
    if (!node) return;
    FreeTree(node->left);
    FreeTree(node->right);
    delete node;
}

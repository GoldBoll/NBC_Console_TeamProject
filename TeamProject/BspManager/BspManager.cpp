#include "BspManager.h"
#include <ctime>
#include <algorithm>

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

    // ① 전체 Wall 로 초기화 (BSP 는 Wall 에서 깎아내는 방식)
    map.Fill(Tile::Wall);

    // ② 트리 빌드 → 방 조각 → 복도 연결
    root = BuildTree(0, 0, MAP_W, MAP_H, 0);
    CarveRooms(root, map);
    ConnectTree(root, map);
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

//  ConnectTree : 형제 방을 L자형 복도(3칸 너비)로 연결 (bottom-up)
//  반환값: 이 서브트리를 대표하는 방 (상위 연결에 사용)
Room BspManager::ConnectTree(BspNode* node, Map& map)
{
    if (!node)       return {};
    if (node->IsLeaf()) return node->room;

    Room l = ConnectTree(node->left,  map);
    Room r = ConnectTree(node->right, map);

    // 두 방의 중심을 L자형 복도로 연결
    // 수평 이동 → 수직 이동
    int x1 = l.CenterX(), y1 = l.CenterY();
    int x2 = r.CenterX(), y2 = r.CenterY();

    HCorridor(map, x1, x2, y1);
    VCorridor(map, x2, y1, y2);

    // 부모가 연결할 때 사용할 대표 방 하나를 랜덤 반환
    return (rng() % 2) ? l : r;
}

//  HCorridor : y 행에서 x1~x2 구간을 칸 높이 Floor 로 연결
void BspManager::HCorridor(Map& map, int x1, int x2, int y)
{
    int from = std::min(x1, x2);
    int to = std::max(x1, x2);
    for (int x = from; x <= to; ++x)
        for (int dy = -1; dy <= 1; ++dy)        // y-1, y, y+1 (중심기준 ±1)
            if (map.InBounds(x, y + dy))
                map.SetTile(x, y + dy, Tile::Floor);
}

//  VCorridor : x 열에서 y1~y2 구간을 2칸 너비 Floor 로 연결
void BspManager::VCorridor(Map& map, int x, int y1, int y2)
{
    int from = std::min(y1, y2);
    int to = std::max(y1, y2);
    for (int y = from; y <= to; ++y)
        for (int dx = -1; dx <= 1; ++dx)        // x-1, x, x+1 (중심기준 ±1)
            if (map.InBounds(x + dx, y))
                map.SetTile(x + dx, y, Tile::Floor);
}

void BspManager::FreeTree(BspNode* node)
{
    if (!node) return;
    FreeTree(node->left);
    FreeTree(node->right);
    delete node;
}

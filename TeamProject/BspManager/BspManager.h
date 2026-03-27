#pragma once
#include "../Map/Map.h"
#include <vector>
#include <random>

// ─────────────────────────────────────────────────────────────────────────────
//  BspManager
//  BSP(Binary Space Partitioning) 알고리즘으로 Map 타일을 채우고,
//  생성된 방 목록(Room)을 외부(GameManager, SpawnManager)에 제공합니다.
//
//  사용 예:
//      BspManager::Params p;
//      p.seed     = 0;      // 0 → 매 실행마다 다른 맵
//      p.maxDepth = 5;      // 깊이 5 ≈ 최대 32개 방
//      BspManager::GetInstance().Generate(map, p);
//
//      const auto& rooms = BspManager::GetInstance().GetRooms();
//      // rooms[0] : 플레이어 스폰 추천
//      // rooms.back() : 계단/보스 스폰 추천
// ─────────────────────────────────────────────────────────────────────────────

class BspManager
{
public:
    //생성 파라미터
    struct Params
    {
        // 파티션 (분할 영역) 최소 크기
        // 이 값을 키우면 → 방이 적고 큼
        // 이 값을 줄이면 → 방이 많고 작음
        int minPartitionW = 20;
        int minPartitionH = 20;

        // 방 크기 범위 (파티션 크기를 초과할 수 없음)
        int minRoomW = 8;
        int minRoomH = 8;
        int maxRoomW = 25;   // 큰 방을 원하면 늘림
        int maxRoomH = 25;

        // 분할 최대 깊이 (2^maxDepth 개까지 리프 노드 생성 가능)
        int maxDepth = 8;

        // 랜덤 시드 (0 이면 time(nullptr) 로 자동 설정)
        unsigned seed = 0;
    };

    static BspManager& GetInstance();

    // map 전체를 BSP 맵으로 생성합니다.
    // 호출 후 GetRooms() 로 방 목록을 얻을 수 있습니다.
    void Generate(Map& map, const Params& params = {});

    const std::vector<Room>& GetRooms() const { return rooms; }

    BspManager(const BspManager&)            = delete;
    BspManager& operator=(const BspManager&) = delete;

private:
    BspManager() = default;

    struct BspNode
    {
        int      x, y, w, h;
        BspNode* left  = nullptr;
        BspNode* right = nullptr;
        Room     room  = {};          // leaf 노드에서만 유효
        bool     IsLeaf() const { return !left && !right; }
    };

    BspNode* BuildTree(int x, int y, int w, int h, int depth);
    void     CarveRooms(BspNode* node, Map& map);
    Room     ConnectTree(BspNode* node, Map& map);  // 재귀 연결 후 대표 방 반환
    void     HCorridor(Map& map, int x1, int x2, int y);
    void     VCorridor(Map& map, int x,  int y1, int y2);
    void     FreeTree(BspNode* node);

    BspNode*          root = nullptr;
    std::vector<Room> rooms;
    std::mt19937      rng;
    Params            curParams;
};

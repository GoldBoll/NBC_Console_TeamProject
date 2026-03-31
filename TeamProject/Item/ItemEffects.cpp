#include "ItemList.h"
#include "../Player/Player.h"
#include "../GameManager/GameManager.h"
#include "../Render/Render.h"
#include <cstdlib>
#include <vector>

// 순환 include로 인해 ItemList.h에서 구현할 수 없는
// Use() 함수들 구현합니다.
// (GameManager / Map / Render 접근이 필요한 아이템)

void HealPotion::Use(Player& player)
{
    player.Heal(10);
}

void TeleportPotion::Use(Player& player)
{
    Map& map = GameManager::GetInstance().GetMap();

    // 맵에서 이동 가능한 Floor 타일 목록 수집
    std::vector<std::pair<int, int>> floorTiles;
    for (int y = 0; y < MAP_H; ++y)
        for (int x = 0; x < MAP_W; ++x)
            if (map.GetTile(x, y) == Tile::Floor)
                floorTiles.push_back({ x, y });

    if (floorTiles.empty())
    {
        Render::GetInstance().AddLog("이동할 수 있는 공간이 없습니다.", CLR_RED);
        return;
    }

    // 랜덤 Floor 타일 선택
    auto& tile = floorTiles[rand() % floorTiles.size()];
    int nx = tile.first;
    int ny = tile.second;

    // 기존 위치를 Floor로, 새 위치를 Player로 교체
    map.SetTile(player.GetX(), player.GetY(), Tile::Floor);
    player.SetX(nx);
    player.SetY(ny);
    map.SetTile(nx, ny, Tile::Player);

    Render::GetInstance().AddLog("랜덤 위치로 순간이동했습니다!", CLR_CYAN);
}

void InvisibilityItem::Use(Player& player)
{
    player.SetInvisibility(5);
    Render::GetInstance().AddLog("5턴 동안 투명화 상태가 됩니다.", CLR_CYAN);
}

void ReviveToken::Use(Player& player)
{
    player.ActivateReviveToken();
    Render::GetInstance().ShowReviveToken();
}

#pragma once

// ============================================================================
//  UILayout.h  - 비율 기반 콘솔 UI 레이아웃
//
//  ┌─────────────────────────┬──────────┬─────────────────┐
//  │                         │  Player  │                 │
//  │                         │   Info   │                 │
//  │          Map            │──────────│      Log        │
//  │                         │  Info 2  │                 │
//  │                         │          │                 │
//  ├─────────────────────────┴──────────┴─────────────────┤
//  │  [help:h]  [inven:i]  ...                            │
//  └──────────────────────────────────────────────────────┘
//
//  ▶ TOTAL_W / TOTAL_H 만 바꾸면 전체 UI 가 비율대로 자동 조정됩니다.
//    TOTAL_W 는 COL_TOTAL 의 배수로 설정하면 정수 분할이 깔끔합니다.
// ============================================================================

// ─── 1. 전체 콘솔 크기 ──────────────────────────────────────────────────────
constexpr int TOTAL_W = 115;    // ◀ 전체 너비  (COL_TOTAL=10 의 배수 권장)
constexpr int TOTAL_H = 32;     // ◀ 전체 높이

// ─── 2. 가로 비율 (합계 = COL_TOTAL) ────────────────────────────────────────
//   Map : Middle : Log  =  COL_MAP : COL_MID : (나머지)
constexpr int COL_TOTAL = 10;
constexpr int COL_MAP   = 5;    // Map 너비 비율
constexpr int COL_MID   = 2;    // 중간 패널(Player Info + Info2) 너비 비율
// Log 는 나머지 전체 사용

// ─── 3. 하단 도움말 바 높이 ──────────────────────────────────────────────────
constexpr int HELP_BAR_H = 3;   // ◀ 도움말 바 높이 (테두리 포함)

// ─── 4. 계산된 패널 너비 ─────────────────────────────────────────────────────
constexpr int MAP_BOX_W = TOTAL_W * COL_MAP / COL_TOTAL;
constexpr int MID_BOX_W = TOTAL_W * COL_MID / COL_TOTAL;
constexpr int LOG_BOX_W = TOTAL_W - MAP_BOX_W - MID_BOX_W;

// ─── 5. 계산된 패널 높이 ─────────────────────────────────────────────────────
constexpr int MAIN_H        = TOTAL_H - HELP_BAR_H; // 메인 패널 공통 높이
constexpr int MAP_BOX_H     = MAIN_H;
constexpr int PLAYER_INFO_H = MAIN_H / 2;           // Player Info 패널 높이
constexpr int INFO2_H       = MAIN_H - PLAYER_INFO_H; // Info 2 패널 높이
constexpr int LOG_BOX_H     = MAIN_H;

// ─── 6. 뷰포트 크기 (Map 박스 내부, 테두리 2칸 제외) ────────────────────────
constexpr int VIEW_W = MAP_BOX_W - 2;
constexpr int VIEW_H = MAP_BOX_H - 2;

// ─── 7. 패널 좌표 ────────────────────────────────────────────────────────────
constexpr int MAP_BOX_X      = 0;
constexpr int MAP_BOX_Y      = 0;

constexpr int PLAYER_INFO_X  = MAP_BOX_W;
constexpr int PLAYER_INFO_Y  = 0;

constexpr int INFO2_BOX_X    = MAP_BOX_W;
constexpr int INFO2_BOX_Y    = PLAYER_INFO_H;

constexpr int LOG_BOX_X      = MAP_BOX_W + MID_BOX_W;
constexpr int LOG_BOX_Y      = 0;

constexpr int HELP_BAR_X     = 0;
constexpr int HELP_BAR_Y     = MAIN_H;
constexpr int HELP_BAR_W     = TOTAL_W;

// ─── 8. 로그 최대 줄 수 ──────────────────────────────────────────────────────
constexpr int LOG_MAX_LINES  = LOG_BOX_H - 2;

#pragma once
#include "Item.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>

class Player;

class Inventory
{
private:
	static const int MAX_SLOTS = 20;
	Item* slots[MAX_SLOTS];

public:
	Inventory();
	~Inventory();

	bool AddItem(Item* newItem); // 아이템 추가
	bool UseItem(int index, Player& player); // 아이템 사용
	bool RemoveItem(int index);  // 아이템 삭제

	// 채워진 슬롯만 반환: (슬롯 인덱스, Item 포인터) 쌍의 벡터
	std::vector<std::pair<int, Item*>> GetFilledItems() const;
};

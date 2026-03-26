#pragma once
#include "Item.h"
#include <iostream>
#include <string>

class Inventory
{
private:
	static const int MAX_SLOTS = 20;
	Item* slots[MAX_SLOTS];

public:
	Inventory();
	~Inventory();

	bool AddItem(Item* newItem); // 아이템 추가
	bool UseItem(int index);     // 아이템 사용
	bool RemoveItem(int index);  // 아이템 삭제
	void ShowInventory() const;  // 인벤토리 출력 (ASCII 스타일)
};
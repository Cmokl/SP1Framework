#pragma once
#include "Items.h"

class Inventory
{
	Items* InventoryItems[10];
	int gold;
	bool IsFull;
public:
	Inventory();
	~Inventory();
	void SetGold(int value);
	int GetGold();
	void AddItem(Items* item);
	void DiscardItem(Items* item);
	Items* GetItem(int ItemIndex);
	bool CheckIsFull();
};

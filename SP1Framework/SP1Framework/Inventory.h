#pragma once
#include <string>
#include <Items.h>
#include <Class.h>

class Inventory
{
	Items* InventoryItems[10];
	int gold;
public:
	Inventory();
	~Inventory();
	void SetGold(int value);
	int GetGold();
	void AddItem(Items* NewItem);
	void DiscardItem(Items* DiscardedItem);
	void UseItem(Items* UsedItem, Class* target);
};
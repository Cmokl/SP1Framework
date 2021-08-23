#pragma once
#include "Items.h"
#include "Class.h"

class Inventory
{
	Items* InventoryItems[10];
	int gold;
public:
	Inventory();
	~Inventory();
	virtual void ItemEffect(Class* Player, int EffectValue);
	void SetGold(int value);
	int GetGold();
	void AddItem(Items* item);
	void DiscardItem(Items * item);
	Items* GetItem(int ItemIndex);
};
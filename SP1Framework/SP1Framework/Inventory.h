#pragma once
#include <string>

class Inventory :
{
	std::string InventoryItems[10];
	int gold;
public:
	Inventory();
	~Inventory();
	void SetGold(int value);
	int GetGold();
	void AddItem(std::string ItemName);
	void DiscardItem(std::string ItemName);
}
#pragma once
#include "Items.h"
#include "Inventory.h"
class ArmourItems : public Items
{
private:
	int ArmourWorth;
public:
	ArmourItems();
	ArmourItems(std::string name, int cost, int ArmourWorth);
	~ArmourItems();
	void ItemFunction(Class* target);
	void RemoveArmour(Class* target);
};
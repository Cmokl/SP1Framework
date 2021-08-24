#pragma once
#include "Items.h"
#include "Class.h"

class HealingItems :
	public Items
{
public:
	HealingItems();
	HealingItems(std::string name, int cost, int HealingWorth);//HealingWorth is the amount of hp a player gets from the item
	~HealingItems();
	void ItemEffect(Class* Player);
};

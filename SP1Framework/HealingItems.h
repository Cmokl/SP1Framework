#pragma once
#include "Items.h"

class HealingItems :
	public Items
{
	int HealingWorth;
public:
	HealingItems();
	HealingItems(std::string name, int cost, int HealingWorth);//HealingWorth is the amount of hp a player gets from the item
	~HealingItems();
	void HealPlayer(Class* target);
};
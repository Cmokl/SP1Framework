#pragma once
#include "Items.h"

class ImmunityItems : public Items
{
	ImmunityItems();
	ImmunityItems(std::string name, int cost, int EffectWorth);
	~ImmunityItems();
	void ItemEffect(Class* Player);
};
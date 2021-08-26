#pragma once
#include "Items.h"
#include "Class.h"

class ImmunityItems : public Items
{
public:
	ImmunityItems();
	ImmunityItems(std::string name, int cost, int EffectWorth);
	~ImmunityItems();
	void ItemEffect(Class* Player);
};
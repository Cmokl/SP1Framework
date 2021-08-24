#pragma once
#include <string>
#include "Class.h"

class Items
{
	int cost;
	int EffectValue;
	std::string name;
	std::string description;
public:
	Items();
	~Items();
	//get and set functions
	void SetCost(int cost);
	int GetCost();
	void SetName(std::string name);
	std::string GetName();
	void SetDescription(std::string NewDescription);
	std::string GetDescription();
	int GetEffectValue();
	void SetEffectValue(int EffectValue);
	//item's effect
	virtual void ItemEffect(Class* Player);
};

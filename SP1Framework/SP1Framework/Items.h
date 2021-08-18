#pragma once
#include <string>
#include "Class.h"
class Items
{
	int cost;
	std::string name;
public:
	Items();
	~Items();
	//get and set functions
	void SetCost(int cost);
	int GetCost();
	void SetName(std::string name);
	std::string GetName();
	//other functions
	virtual void ItemFunction(Class* target);// Its subclasses, healing items, weapons and armour have different item functions
};
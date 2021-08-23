#pragma once
#include <string>
class Items
{
	int cost;
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
};
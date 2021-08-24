#include "Items.h"

Items::Items()
{
	cost = 0;
	name = "-";
	description = "-";
}
Items::~Items()
{
}
//get and set functions

void Items::SetCost(int cost)
{
	this->cost = cost;
}
int Items::GetCost()
{
	return cost;
}
void Items::SetName(std::string name)
{
	this->name = name;
}
std::string Items::GetName()
{
	return name;
}
void Items::SetDescription(std::string NewDescription)
{
	description = NewDescription;
}
std::string Items::GetDescription()
{
	return description;
}

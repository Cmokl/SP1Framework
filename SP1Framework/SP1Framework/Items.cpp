#include "Items.h"

Items::Items()
{
	cost = 0;
	name = "-";
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
//other functions
virtual void Items::ItemFunction(Class* target)
{
}
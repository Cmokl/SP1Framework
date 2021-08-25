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
int Items::GetEffectValue()
{
	return EffectValue;
}
void Items::SetEffectValue(int EffectValue)
{
	this->EffectValue = EffectValue;
}
//Item's effect
void Items::ItemEffect(Class* Player)
{
}

#include "ImmunityItems.h"


ImmunityItems::ImmunityItems()
{
}
ImmunityItems::ImmunityItems(std::string name, int cost, int EffectWorth)
{
	this->SetName(name);
	this->SetCost(cost);
	this->SetEffectValue(EffectWorth);
}
ImmunityItems::~ImmunityItems()
{
}
void ImmunityItems::ItemEffect(Class* Player)
{
	Player->SetIsImmune(true);
}
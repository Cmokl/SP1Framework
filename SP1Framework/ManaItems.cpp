#include "ManaItems.h"
ManaItems::ManaItems()
{
}
ManaItems::ManaItems(std::string name, int cost, int ManaWorth)
{
	this->SetName(name);
	this->SetCost(cost);
	this->SetEffectValue(ManaWorth);
}
ManaItems::~ManaItems()
{
}
void ManaItems::ItemEffect(Class* Player)
{
	Player->SetMana(Player->GetMana() + this->GetEffectValue());
	if (Player->GetMaxMana() > Player->GetMana())
	{
		Player->SetMana(Player->GetMaxMana());
	}
}

#include "HealingItems.h"
#include "Class.h"

HealingItems::HealingItems()
{
	SetEffectValue(0);
}
HealingItems::HealingItems(std::string name, int cost, int HealingWorth)
{
	this->SetName(name);
	this->SetCost(cost);
	this->SetEffectValue(HealingWorth);
}
HealingItems::~HealingItems()
{

}
void HealingItems::ItemEffect(Class* player)
{
	player->SetHealth(player->GetHealth() + this->GetEffectValue());
	if (player->GetHealth() > player->GetMaxHealth())
	{
		player->SetHealth(player->GetMaxHealth());
	}
}

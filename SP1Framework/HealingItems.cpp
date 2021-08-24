#include "HealingItems.h"
#include "Class.h"

HealingItems::HealingItems()
{
	HealingWorth = 0;
}
HealingItems::HealingItems(std::string name, int cost, int HealingWorth)
{
	this->SetName(name);
	this->SetCost(cost);
	this->HealingWorth = HealingWorth;
}
HealingItems::~HealingItems()
{

}
void HealingItems::ItemEffect(Class* player, int EffectValue)
{
	player->SetHealth(player->GetHealth() + HealingWorth);
	if (player->GetHealth() > player->GetMaxHealth())
	{
		player->SetHealth(player->GetMaxHealth());
	}
}

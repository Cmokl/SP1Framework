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
void HealingItems::HealPlayer(Class* target)
{
	target->SetHealth(target->GetHealth() + HealingWorth);
	if (target->GetHealth() > target->GetMaxHealth())
	{
		target->SetHealth(target->GetMaxHealth());
	}
}
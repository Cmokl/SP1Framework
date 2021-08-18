#include "ArmourItems.h"

ArmourItems::ArmourItems()
{
}
ArmourItems::ArmourItems(std::string name, int cost, int ArmourWorth)
{
	this->SetName(name);
	this->SetCost(cost);
	this->ArmourWorth = ArmourWorth;
}
ArmourItems::~ArmourItems()
{

}
void ArmourItems::ItemFunction(Class* target)
{
	//equipping item
	target->SetArmourStatus(true, ArmourWorth);
}
void ArmourItems::RemoveArmour(Class* target)
{
	target->SetArmourStatus(false, ArmourWorth);
}
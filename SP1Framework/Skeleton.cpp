#include "Skeleton.h"

Skeleton::Skeleton()
{
	this->SetHealth(15);
	this->SetMaxHealth(15);
	this->SetMana(15);
	this->SetStrength(9);
	this->SetIntelligence(6);
	this->SetFaith(6);
	this->SetSpeed(13);
	this->SetDefence(10);
	this->SetResistance(8);
}
Skeleton::~Skeleton()
{
}
void Skeleton::Pierce(Class* target)
{
	target->SetHealth(target->GetHealth() - 6 + 6 * (0.1 * (target->GetDefence() / 10)));
}
void Skeleton::Reassemble()
{
	this->SetHealth(GetHealth() + 4);
}
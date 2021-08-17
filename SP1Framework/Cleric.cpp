#include "Cleric.h"
Cleric::Cleric()
{
	this->SetHealth(28);
	this->SetMaxHealth(28);
	this->SetMana(40);
	this->SetStrength(8);
	this->SetIntelligence(13);
	this->SetFaith(16);
	this->SetSpeed(10);
	this->SetDefence(10);
	this->SetResistance(12);
}
Cleric::~Cleric()
{
}
void Cleric::HolyRestoration(Class* target)
{
	target->SetHealth(target->GetHealth() + (GetFaith()/2)); 
	if (target-> GetHealth() > target->GetMaxHealth())
	{
		target->SetMaxHealth(target->GetMaxHealth());
	}
}
void Cleric::Resurrection(Class* target)
{
	target->SetMaxHealth(GetMaxHealth() * 0.1);
}
void Cleric::Protection(Class* target1, Class* target2, Class* target3, Class* target4)
{
	target1->SetIsImmune;
	target2->SetIsImmune;
	target3->SetIsImmune;
	target4->SetIsImmune;
}
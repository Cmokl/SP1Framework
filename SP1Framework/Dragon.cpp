#include "Dragon.h"

Dragon::Dragon()
{
	this->SetHealth(55);
	this->SetMaxHealth(55);
	this->SetMana(40);
	this->SetStrength(16);
	this->SetIntelligence(10);
	this->SetFaith(8);
	this->SetSpeed(16);
	this->SetDefence(15);
	this->SetResistance(15);
	DragonsCallUsed = 0;
}
Dragon::~Dragon()
{
}
void Dragon::FireBreath(Class* target)
{
	target->SetHealth(target->GetHealth() - 10 + 10 * (0.1 * (target->GetDefence() / 10)));
	target->SetIsBurn(true);
	//to add the player burning for 3 turns
}
void Dragon::TalonTear(Class* target)
{
	target->SetHealth(target->GetHealth() - 9 + 9 * (0.1 * (target->GetDefence() / 10)));
}
void Dragon::FlameBurst(Class* target1, Class* target2, Class* target3, Class* target4)
{
	target1->SetHealth(target1->GetHealth() - 9 + 9 * (0.1 * (target2->GetDefence() / 10)));
	target2->SetHealth(target2->GetHealth() - 9 + 9 * (0.1 * (target2->GetDefence() / 10)));
	target3->SetHealth(target3->GetHealth() - 9 + 9 * (0.1 * (target3->GetDefence() / 10)));
	target4->SetHealth(target4->GetHealth() - 9 + 9 * (0.1 * (target4->GetDefence() / 10)));
}
void Dragon::DragonsCall(Oni* NewEnemy)
{
	if (DragonsCallUsed == 0 && (GetHealth() <= (GetMaxHealth() * 0.25)))
	{
		NewEnemy = new Oni;
		DragonsCallUsed = 1;
	}
}


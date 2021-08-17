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
	target->SetHealth(target->GetHealth - 10);
	//to add the player burning for 3 turns
}
void Dragon::TalonTear(Class* target)
{
	target->SetHealth(target->GetHealth() - 8);
}
void Dragon::FlameBurst(Class* target1, Class* target2, Class* target3, Class* target4)
{
	target1->SetHealth(target->GetHealth - 9);
	target2->SetHealth(target->GetHealth - 9);
	target3->SetHealth(target->GetHealth - 9);
	target4->SetHealth(target->GetHealth - 9);
}
void Dragon::DragonsCall(Oni* NewEnemy)
{
	if (DragonsCallUsed == 0 && (GetHealth() <= (GetMaxHealth() * 0.25)));
	{
		NewEnemy = new Oni;
		DragonsCallUsed = 1;
	}
}
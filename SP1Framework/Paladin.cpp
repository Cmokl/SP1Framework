#include "Paladin.h"
Paladin::Paladin()
{
	this->SetHealth(35);
	this->SetMaxHealth(35);
	this->SetMana(35);
	this->SetStrength(13);
	this->SetIntelligence(9);
	this->SetFaith(15);
	this->SetSpeed(13);
	this->SetDefence(13);
	this->SetResistance(9);

	IsFlame = false;
	GuardedIndex = 0;
}
Paladin::~Paladin()
{

}

void Paladin::Attack(Class* Target)
{

}

void Paladin::Guard(Class* Party[])
{
	for (int i = 0; i < 4; i++)
	{
	}
}

void Paladin::RevertGuard(Class* Party[])
{

}

void Paladin::InspiringAura(Class* Self[])
{
	Class* Temp;
	for (int i = 0; i < 4; i++)
	{ 
		Temp = Self[i];
		Temp->SetResistance(Temp->GetResistance() * 1.15);
	}
}

void Paladin::RevertInspiringAura(Class* Self[])
{
	Class* Temp;
	for (int i = 0; i < 4; i++)
	{
		Temp = Self[i];
		Temp->SetResistance(Temp->GetResistance() / 1.15);
	}
}

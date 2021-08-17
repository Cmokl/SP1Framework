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

void Paladin::Guard(Class* Target, Party* Self)
{
	Class* Temp;
	for (int i = 0; i < 4; i++)
	{
		Temp = Self->GetPartyClass(i);

		if (Temp == Target)
		{
			Self->SetPartyClass(i, this);
		}
	}
}

void Paladin::RevertGuard(Class* Target, Party* Self)
{
	Self->SetPartyClass(GuardedIndex, Target);
}

void Paladin::InspiringAura(Party* Self)
{
	Class* Temp;
	for (int i = 0; i < 4; i++)
	{ 
		Temp = Self->GetPartyClass(i);
		Temp->SetResistance(Temp->GetResistance() * 1.15);
	}
}

void Paladin::RevertInspiringAura(Party* Self)
{
	Class* Temp;
	for (int i = 0; i < 4; i++)
	{
		Temp = Self->GetPartyClass(i);
		Temp->SetResistance(Temp->GetResistance() / 1.15);
	}
}

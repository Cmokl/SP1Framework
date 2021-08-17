#include "Rogue.h"

Rogue::Rogue()
{
	this->SetHealth(35);
	this->SetMana(30);
	this->SetStrength(13);
	this->SetIntelligence(9);
	this->SetFaith(8);
	this->SetSpeed(16);
	this->SetDefence(12);
	this->SetResistance(10);

	IsStealth = false;
}
Rogue::~Rogue()
{
}

void Rogue::Attack(Class* Target)
{
	Target->SetHealth(Target->GetHealth() - (this->GetStrength() * 1.0 + Target->GetDefence() * 0.5));
}

void Rogue::Defend()
{
	this->SetDefence(this->GetDefence() * 1.5);
	this->SetResistance(this->GetResistance() * 1.5);
}


#include "Rogue.h"

Rogue::Rogue()
{
	this->SetHealth(35);
	this->SetMaxHealth(35);
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


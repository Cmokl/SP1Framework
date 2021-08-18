#include "Medusa.h"
Medusa::Medusa()
{

}
Medusa::~Medusa()
{
	this->SetHealth(24);
	this->SetMana(25);
	this->SetStrength(12);
	this->SetIntelligence(13);
	this->SetFaith(8);
	this->SetSpeed(10);
	this->SetDefence(12);
	this->SetResistance(10);
}
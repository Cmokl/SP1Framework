#include "Medusa.h"
Medusa::Medusa()
{
	this->SetHealth(24);
	this->SetMaxHealth(25);
	this->SetMana(25);
	this->SetStrength(12);
	this->SetIntelligence(13);
	this->SetFaith(8);
	this->SetSpeed(10);
	this->SetDefence(12);
	this->SetResistance(10);
}
Medusa::~Medusa()
{

}
void Medusa::CursedGaze(Class* target)
{
	target->SetHealth(target->GetHealth() - 7 + (7 * 0.1*(target->GetDefence() / 10)));
}
void Medusa::SerpentBite(Class* target)
{
	target->SetHealth(target->GetHealth() - 6 + (6 * 0.1*(target->GetDefence() / 10)));
	target->SetIsPoison(true);
}
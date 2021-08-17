#include "Ghoul.h"

Ghoul::Ghoul()
{
	this->SetHealth(20);
	this->SetMaxHealth(20);
	this->SetMana(20);
	this->SetStrength(10);
	this->SetIntelligence(10);
	this->SetFaith(10);
	this->SetSpeed(10);
	this->SetDefence(10);
	this->SetResistance(10);
}
Ghoul::~Ghoul()
{
}

void Ghoul::Devour(Class* Target)
{
	int damage = (this->GetStrength() * 1.0 + Target->GetDefence() * 0.5);
	//damage
	Target->SetHealth(Target->GetHealth() - damage);
	//heal
	for (int i = 0; i < damage; i++)
	{
		if (this->GetMaxHealth() != this->GetHealth())
		{
			this->SetHealth(this->GetHealth() + 1);
		}
	}
}

void Ghoul::Shapeshift(void)
{
	this->SetDefence(this->GetHealth() + 2);
}

void Ghoul::RevertShapeshift(void)
{
	this->SetDefence(this->GetHealth() - 2);
}

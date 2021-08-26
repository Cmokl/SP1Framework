#include "Ghoul.h"

Ghoul::Ghoul()
{
	this->SetHealth(20);
	this->SetMaxHealth(20);
	this->SetStrength(12);
	this->SetIntelligence(10);
	this->SetFaith(10);
	this->SetSpeed(10);
	this->SetDefence(10);
	this->SetResistance(10);

	IsShapeshift = false;
}
Ghoul::~Ghoul()
{
}

bool Ghoul::GetIsShapeshift(void)
{
	return IsShapeshift;
}

void Ghoul::SetIsShapeshift(bool boolean)
{
	IsShapeshift = boolean;
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
	IsShapeshift = true;
	this->SetDefence(this->GetDefence() + 5);
}

void Ghoul::RevertShapeshift(void)
{
	IsShapeshift = false;
	this->SetDefence(this->GetDefence() - 5);
}

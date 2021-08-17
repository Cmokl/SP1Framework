#include "Class.h"

Class::Class()
{
	Health = 0;
	Mana = 0;
	Strength = 0;
	Intelligence = 0;
	Faith = 0;
	Speed = 0;
	Defence = 0;
	Resistance = 0;
	MaxHealth = 0;
}

Class::~Class()
{
}

//setter/getters
int Class::GetHealth(void)
{
	return Health;
}
void Class::SetHealth(int Value)
{
	Health = Value;
}

int Class::GetMana(void)
{
	return Mana;
}

void Class::SetMana(int Value)
{
	Mana = Value;
}

int Class::GetStrength(void)
{
	return Strength;
}

void Class::SetStrength(int Value)
{
	Strength = Value;
}

int Class::GetIntelligence(void)
{
	return Intelligence;
}

void Class::SetIntelligence(int Value)
{
	Intelligence = Value;
}

int Class::GetFaith(void)
{
	return Faith;
}

void Class::SetFaith(int Value)
{
	Faith = Value;
}

int Class::GetSpeed(void)
{
	return Speed;
}

void Class::SetSpeed(int Value)
{
	Speed = Value;
}

int Class::GetDefence(void)
{
	return Defence;
}

void Class::SetDefence(int Value)
{
	Defence = Value;
}

int Class::GetResistance(void)
{
	return Resistance;
}

void Class::SetResistance(int Value)
{
	Resistance = Value;
}

//functions
void Class::Attack(Class* Target)
{
	Target->SetHealth(Target->GetHealth() - (this->GetStrength() * 1.0 + Target->GetDefence() * 0.5));
}

void Class::Defend()
{
	this->SetDefence(this->GetDefence() * 1.5);
	this->SetResistance(this->GetResistance() * 1.5);
}
int Class::GetMaxHealth()
{
	return Health;
}
void Class::SetMaxHealth(int Value)
{
	setMaxHealth(Value);
}
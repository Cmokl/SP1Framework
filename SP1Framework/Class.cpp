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
}

Class::~Class()
{
}

void Class::GetHealth(void)
{
}
int Class::SetHealth(int Value)
{
	return Health;
}

void Class::GetMana(void)
{
}

int Class::SetMana(int Value)
{
	return Mana;
}

void Class::GetStrength(void)
{
}

int Class::SetStrength(int Value)
{
	return Strength;
}

void Class::GetIntelligence(void)
{
}

int Class::SetIntelligence(int Value)
{
	return Intelligence;
}

void Class::GetFaith(void)
{
}

int Class::SetFaith(int Value)
{
	return Faith;
}

void Class::GetSpeed(void)
{
}

int Class::SetSpeed(int Value)
{
	return Speed;
}

void Class::GetDefence(void)
{
}

int Class::SetDefence(int Value)
{
	return Defence;
}

void Class::GetResistance(void)
{
}

int Class::SetResistance(int Value)
{
	return Resistance;
}

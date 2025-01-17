#include "Class.h"

Class::Class()
{
	Name = "";
	MaxHealth = 0;
	Health = 0;
	Mana = 0;
	Strength = 0;
	Intelligence = 0;
	Faith = 0;
	Speed = 0;
	Defence = 0;
	Resistance = 0;

	IsBleed = false;
	IsBurn = false;
	IsPoison = false;
	IsImmune = false;
	IsSilenced = false;
	IsDefend = false;

	Turn = true;
}

Class::~Class()
{
}

//setter/getters(stats)
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

int Class::GetMaxHealth()
{
	return MaxHealth;
}

void Class::SetMaxHealth(int Value)
{
	MaxHealth = Value;
}

int Class::GetMaxMana()
{
	return MaxMana;
}

void Class::SetMaxMana(int Value)
{
	MaxMana = Value;
}

//setter/getters(statuses)
bool Class::GetIsBleed(void)
{
	return IsBleed;
}

void Class::SetIsBleed(bool Boolean)
{
	IsBleed = Boolean;
}
bool Class::GetIsPoison(void)
{
	return IsPoison;
}
void Class::SetIsPoison(bool Boolean)
{
	IsPoison = Boolean;
}
bool Class::GetIsBurn(void)
{
	return IsBurn;
}
void Class::SetIsBurn(bool Boolean)
{
	IsBurn = Boolean;
}
bool Class::GetIsImmune(void)
{
	return IsImmune;
}
void Class::SetIsImmune(bool Boolean)
{
	IsImmune = Boolean;
}

bool Class::GetIsSilenced(void)
{
	return IsSilenced;
}

void Class::SetIsSilenced(bool Boolean)
{
	IsSilenced = Boolean;
}

bool Class::GetIsDefend(void)
{
	return IsDefend;
}

void Class::SetIsDefend(bool Boolean)
{
	IsDefend = Boolean;
}

bool Class::GetTurn(void)
{
	return Turn;
}

void Class::SetTurn(bool Boolean)
{
	Turn = Boolean;
}

//getter setters for Name
std::string Class::GetName(void)
{
	return Name;
}

void Class::SetName(std::string str)
{
	Name = str;
}

//functions
void Class::Attack(Class* Target)
{
	Target->SetHealth(static_cast<int>(Target->GetHealth() - (this->GetStrength() * 1.0 + Target->GetDefence() * 0.5)));
}

void Class::Defend()
{
	this->SetDefence(static_cast<int>(this->GetDefence() * 1.5));
	this->SetResistance(static_cast<int>(this->GetResistance() * 1.5));
	IsDefend = true;
}

void Class::RevertDefend()
{
	this->SetDefence(static_cast<int>(this->GetDefence() * 0.5/1.5));
	this->SetResistance(static_cast<int>(this->GetResistance() * 0.5/1.5));
	IsDefend = false;
}

int Class::SkillTargetType(int ListIndex)
{
	return 0;
}

int Class::ManaCost(int ListIndex)
{
	return 0;
}


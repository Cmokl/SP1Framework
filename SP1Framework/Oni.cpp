#include "Oni.h"

Oni::Oni()
{
	this->SetHealth(48);
	this->SetMaxHealth(48);
	this->SetMana(45);
	this->SetStrength(12);
	this->SetIntelligence(14);
	this->SetFaith(15);
	this->SetSpeed(15);
	this->SetDefence(15);
	this->SetResistance(14);
}
Oni::~Oni()
{
}

void Oni::InfernalBlast(Class* Target)
{
	int damage = (this->GetIntelligence() * 1.0 + Target->GetResistance() * 0.5);
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

void Oni::SoulLock(Class* Target)
{
	Target->SetIsSilenced(true);
}

void Oni::RevertSoulLock(Class* Target)
{
	Target->SetIsSilenced(false);
}

//skill list
bool Oni::SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4])
{
	if (ListIndex == 0)
	{
		if (TargetParty[ClassIndex] != nullptr)
		{
			InfernalBlast(TargetParty[ClassIndex]);
		}
	}
	else if (ListIndex == 1)
	{
		if (TargetParty[ClassIndex] != nullptr)
		{
			SoulLock(TargetParty[ClassIndex]);
		}
	}
}

std::string Oni::SkillNameList(int ListIndex)
{
	if (ListIndex == 0)
	{
		return "Infernal Blast";
	}
	else if (ListIndex == 1)
	{
		return "Soul Lock";
	}

	return "";
}
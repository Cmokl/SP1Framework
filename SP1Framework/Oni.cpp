#include "Oni.h"

Oni::Oni()
{
	this->SetHealth(48);
	this->SetMaxHealth(48);
	this->SetMana(45);
	this->SetStrength(16);
	this->SetIntelligence(15);
	this->SetFaith(15);
	this->SetSpeed(15);
	this->SetDefence(15);
	this->SetResistance(14);
}
Oni::~Oni()
{
}

void Oni::InfenalBlast(Class* Target)
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
void Oni::SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4])
{
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
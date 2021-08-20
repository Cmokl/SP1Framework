#include "Cleric.h"
Cleric::Cleric()
{
	this->SetName("Cleric");
	this->SetHealth(28);
	this->SetMaxHealth(28);
	this->SetMana(40);
	this->SetStrength(8);
	this->SetIntelligence(13);
	this->SetFaith(16);
	this->SetSpeed(10);
	this->SetDefence(10);
	this->SetResistance(12);
}
Cleric::~Cleric()
{
}
void Cleric::HolyRestoration(Class* target)
{
	target->SetHealth(target->GetHealth() + (GetFaith()/2)); 
	if (target-> GetHealth() > target->GetMaxHealth())
	{
		target->SetMaxHealth(target->GetMaxHealth());
	}
}
void Cleric::Resurrection(Class* target)
{
	target->SetMaxHealth(GetMaxHealth() * 0.1);
}
void Cleric::Protection(Class* TargetParty[4])
{
	for (int i = 0; i < 4; i++)
	{
		if (TargetParty[i] != nullptr)
		{
			TargetParty[i]->SetIsImmune(true);
		}
	}
}

void Cleric::SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4])
{
	if (ListIndex == 0)
	{
		HolyRestoration(TargetParty[ClassIndex]);
	}
	else if (ListIndex == 1)
	{
		Resurrection(TargetParty[ClassIndex]);
	}
	else if (ListIndex == 2)
	{
		Protection(TargetParty);
	}
}

std::string Cleric::SkillNameList(int ListIndex)
{
	if (ListIndex == 0)
	{
		return "Holy Restoration";
	}
	else if (ListIndex == 1)
	{
		return "Resurrection";
	}
	else if (ListIndex == 2)
	{
		return "Protection";
	}

	return "";
}

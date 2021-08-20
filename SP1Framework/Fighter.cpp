#include "Fighter.h"

Fighter::Fighter()
{
	//Stat init
	this->SetHealth(40);
	this->SetMaxHealth(40);
	this->SetMana(20);
	this->SetStrength(16);
	this->SetIntelligence(8);
	this->SetFaith(8);
	this->SetSpeed(13);
	this->SetDefence(12);
	this->SetResistance(10);
}

Fighter::~Fighter()
{
}

//skills
void Fighter::Cleave(Class* Target[])
{
	//mana cost 4
	this->SetMana(this->GetMana() - 4);

	for (int i = 0; i < 4; i++)
	{
		if (Target[i] != nullptr)
		{
			Target[i]->SetHealth(Target[i]->GetHealth() - (this->GetStrength() * 0.7 + ((this->GetStrength() * 0.7) * (Target[i]->GetDefence() * 0.05))));
		}
	}
}

void Fighter::Smash(Class* Target)
{
	//mana cost 3
	this->SetMana(this->GetMana() - 3);

	Target->SetHealth(Target->GetHealth() - (this->GetStrength() * 1.4 + ((this->GetStrength() * 1.4) * (Target->GetDefence() * 0.05))));
}

void Fighter::BattleCry(void)
{
	//mana cost 3
	this->SetMana(this->GetMana() - 3);

	this->SetStrength(this->GetStrength() * 1.1);
}

void Fighter::BattleCryRevert()
{
	this->SetStrength(this->GetStrength() *(100/110));
}

void Fighter::SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4])
{
	if (ListIndex == 0)
	{
		Cleave(TargetParty);
	}
	else if (ListIndex == 1)
	{
		Smash(TargetParty[ClassIndex]);
	}
	else if (ListIndex == 2)
	{
		BattleCry();
	}
}

std::string Fighter::SkillNameList(int ListIndex)
{
	if (ListIndex == 0)
	{
		return "Cleave";
	}
	else if (ListIndex == 1)
	{
		return "Smash";
	}
	else if (ListIndex == 2)
	{
		return "Battle Cry";
	}

	return "";
}

#include "Fighter.h"

Fighter::Fighter()
{
	//Stat init
	this->SetName("Fighter");
	this->SetHealth(40);
	this->SetMaxHealth(40);
	this->SetMana(20);
	this->SetMaxMana(20);
	this->SetStrength(16);
	this->SetIntelligence(8);
	this->SetFaith(8);
	this->SetSpeed(13);
	this->SetDefence(12);
	this->SetResistance(10);
	IsBattleCry = false;
}

Fighter::~Fighter()
{
}

bool Fighter::GetIsBattleCry()
{
	return IsBattleCry;
}

//skills
void Fighter::Cleave(Class* Target[4])
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
	if (IsBattleCry == false)
	{
		//mana cost 3
		this->SetMana(this->GetMana() - 3);

		this->SetStrength(this->GetStrength() * 1.1);

		IsBattleCry = true;
	}
}

void Fighter::BattleCryRevert()
{
	this->SetStrength(this->GetStrength() *(100/110));

	IsBattleCry = false;
}

bool Fighter::SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4])
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
	return true;
}

int Fighter::SkillTargetType(int ListIndex)
{
	if (ListIndex == 0)
	{
		return AOE;
	}
	else if (ListIndex == 1)
	{
		return Single;
	}
	else if (ListIndex == 2)
	{
		return Self;
	}
	return Self;
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

int Fighter::ManaCost(int ListIndex)
{
	if (ListIndex == 0)
	{
		return 4;
	}
	else if (ListIndex == 1)
	{
		return 3;
	}
	else if (ListIndex == 2)
	{
		return 3;
	}
	return 0;
}

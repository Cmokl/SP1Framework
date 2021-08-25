#include "Cleric.h"
Cleric::Cleric()
{
	this->SetName("Cleric");
	this->SetHealth(28);
	this->SetMaxHealth(28);
	this->SetMana(40);
	this->SetMaxMana(40);
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

bool Cleric::HolyRestoration(Class* target)
{
	if (target->GetHealth() != target->GetMaxHealth())
	{
		//mana cost 3
		this->SetMana(GetMana() - 3);

		for (int i = 0; i < (this->GetFaith() * 0.8); i++)
		{
			if (target-> GetHealth() + 1 < target->GetMaxHealth())
			{
				target->SetHealth(target->GetHealth() + 1);
			}
		}
		return true;
	}

	return false;
}

bool Cleric::Resurrection(Class* target)
{
	if (target->GetHealth() < 1)
	{
		this->SetMana(GetMana() - 5);

		target->SetHealth(GetMaxHealth() * 0.1);

		return true;
	}

	return false;
}

void Cleric::Protection(Class* TargetParty[])
{
	this->SetMana(GetMana() - 20);

	for (int i = 0; i < 4; i++)
	{
		if (TargetParty[i] != nullptr)
		{
			TargetParty[i]->SetIsImmune(true);
		}
	}
}

bool Cleric::SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4])
{
	if (ListIndex == 0)
	{
		return HolyRestoration(TargetParty[ClassIndex]);
	}
	else if (ListIndex == 1)
	{
		return Resurrection(TargetParty[ClassIndex]);
	}
	else if (ListIndex == 2)
	{
		Protection(TargetParty);
	}
	return true;
}

int Cleric::SkillTargetType(int ListIndex)
{
	if (ListIndex == 0)
	{
		return FSingle;
	}
	else if (ListIndex == 1)
	{
		return FSingle;
	}
	else if (ListIndex == 2)
	{
		return FAOE;
	}
	return Self;
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

int Cleric::ManaCost(int ListIndex)
{
	if (ListIndex == 0)
	{
		return 3;
	}
	else if (ListIndex == 1)
	{
		return 5;
	}
	else if (ListIndex == 2)
	{
		return 20;
	}
	return 0;
}

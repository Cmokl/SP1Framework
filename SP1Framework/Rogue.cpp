#include "Rogue.h"

Rogue::Rogue()
{
	this->SetHealth(25);
	this->SetMaxHealth(25);
	this->SetMana(30);
	this->SetMaxMana(30);
	this->SetStrength(13);
	this->SetIntelligence(9);
	this->SetFaith(8);
	this->SetSpeed(16);
	this->SetDefence(12);
	this->SetResistance(10);

	IsStealth = false;
}
Rogue::~Rogue()
{
}

//getter
bool Rogue::GetIsStealth(void)
{
	return IsStealth;
}

//attack
void Rogue::Attack(Class* Target)
{
	if (IsStealth == false)
	{
		Target->SetHealth(Target->GetHealth() - (this->GetStrength() * 1.0 + Target->GetDefence() * 0.5));
	}
	else
	{
		Target->SetHealth(Target->GetHealth() - (this->GetStrength() * 1.5 + Target->GetDefence() * 0.5));
		IsStealth = false;
	}
}

//skills
void Rogue::Stealth()
{
	//mana cost 2
	this->SetMana(GetMana() - 2);

	//effect
	IsStealth = true;
}

void Rogue::CheapShot(Class* Target)
{
	if (IsStealth == true)
	{
		//mana cost 3
		this->SetMana(GetMana() - 3);

		//effect
		Target->SetHealth(Target->GetHealth() - (this->GetStrength() * 0.9));
	}
}

void Rogue::Lacerate(Class* Target)
{
	if (IsStealth == true)
	{
		//mana cost 3
		this->SetMana(GetMana() - 3);

		//effect
		Target->SetHealth(Target->GetHealth() - (this->GetStrength() * 1.2 + Target->GetDefence() * 0.5));
		Target->SetIsBleed(true);
	}
}

void Rogue::SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4])
{
	if (ListIndex == 0)
	{
		Stealth();
	}
	else if (ListIndex == 1)
	{
		CheapShot(TargetParty[ClassIndex]);
	}
	else if (ListIndex == 2)
	{
		Lacerate(TargetParty[ClassIndex]);
	}
}

int Rogue::SkillTargetType(int ListIndex)
{
	if (ListIndex == 0)
	{
		return Self;
	}
	else if (ListIndex == 1)
	{
		return Single;
	}
	else if (ListIndex == 2)
	{
		return Single;
	}
	return Self;
}

std::string Rogue::SkillNameList(int ListIndex)
{
	if (ListIndex == 0)
	{
		return "Stealth";
	}
	else if (ListIndex == 1)
	{
		return "Cheap Shot";
	}
	else if (ListIndex == 2)
	{
		return "Lacerate";
	}
	return "";
}

int Rogue::ManaCost(int ListIndex)
{
	if (ListIndex == 0)
	{
		return 2;
	}
	else if (ListIndex == 1)
	{
		return 2;
	}
	else if (ListIndex == 2)
	{
		return 3;
	}
	return 0;
}



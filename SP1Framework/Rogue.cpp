#include "Rogue.h"

Rogue::Rogue()
{
	this->SetHealth(35);
	this->SetMaxHealth(35);
	this->SetMana(30);
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
	Class* RogueIndex;

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
void Rogue::Stealth(Party* Target)
{
	Class* RogueIndex;

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



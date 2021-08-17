#include "Fighter.h"

Fighter::Fighter()
{
	//Stat init
	this->SetHealth(40);
	this->SetMana(20);
	this->SetStrength(16);
	this->SetIntelligence(8);
	this->SetFaith(8);
	this->SetSpeed(13);
	this->SetDefence(12);
	this->SetResistance(10);

	OriginalStrength = this->GetStrength();
}

Fighter::~Fighter()
{
}

//skills
void Fighter::Cleave(Party* Target)
{
	Class* Targeted;

	//mana cost 4
	this->SetMana(this->GetMana() - 4);

	for (int i = 0; i < 4; i++)
	{
		Targeted = Target->GetPartyClass(i);

		if (Targeted != nullptr)
		{
			Targeted->SetHealth(Targeted->GetHealth() - (this->GetStrength() * 0.7 + ((this->GetStrength() * 0.8) * (Targeted->GetDefence() * 0.05))));
		}
	}
}

void Fighter::Smash(Class* Target)
{
	//mana cost 3
	this->SetMana(this->GetMana() - 3);

	Target->SetHealth(Target->GetHealth() - (this->GetStrength() * 1.4 + ((this->GetStrength() * 0.8) * (Target->GetDefence() * 0.05))));
}

void Fighter::BattleCry(void)
{
	//mana cost 3
	this->SetMana(this->GetMana() - 3);

	this->SetStrength(this->GetStrength() * 1.1);
}

void Fighter::BattleCryRevert()
{
	this->SetStrength(OriginalStrength);
}

#include "Wizard.h"
Wizard::Wizard()
{
	this->SetHealth(25);
	this->SetMana(40);
	this->SetStrength(8);
	this->SetIntelligence(16);
	this->SetFaith(13);
	this->SetSpeed(13);
	this->SetDefence(9);
	this->SetResistance(12);
}
Wizard::~Wizard()
{
}

void Wizard::MagicMissile(Party* Target)
{
	Class* Targeted;
	//mana cost 3
	this->SetMana(GetMana() - 3);

	srand(static_cast<unsigned int>(time(0)));
	for (int i = 0; i < 3; i++)
	{
		Targeted = Target->GetPartyClass(rand() % 3);
		Targeted->SetHealth(Targeted->GetHealth() - (this->GetIntelligence() * 0.9 + ((this->GetIntelligence() * 0.9) * (Targeted->GetResistance() * 0.05))));
	}
}



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

	IsMirror = false;
}
Wizard::~Wizard()
{
}

//skills
void Wizard::MagicMissile(Party* Target)
{
	Class* Targeted;
	//mana cost 3
	this->SetMana(GetMana() - 3);

	//effect
	srand(static_cast<unsigned int>(time(0)));
	for (int i = 0; i < 3; i++)
	{
		Targeted = Target->GetPartyClass(rand() % 3);
		Targeted->SetHealth(Targeted->GetHealth() - (this->GetIntelligence() * 0.9 + ((this->GetIntelligence() * 0.9) * (Targeted->GetResistance() * 0.05))));
	}

	if (IsMirror == true)
	{
		srand(static_cast<unsigned int>(time(0)));
		for (int i = 0; i < 3; i++)
		{
			Targeted = Target->GetPartyClass(rand() % 3);
			Targeted->SetHealth(Targeted->GetHealth() - (this->GetIntelligence() * 0.9 + ((this->GetIntelligence() * 0.9) * (Targeted->GetResistance() * 0.05))));
		}
		IsMirror = false;
	}
}

void Wizard::PyroBlast(Class* Target)
{
	//mana cost 4
	this->SetMana(GetMana() - 3);

	//effect
	Target->SetHealth(Target->GetHealth() - (this->GetIntelligence() * 2.1 + ((this->GetIntelligence() * 2.1) * (Target->GetResistance() * 0.05))));

	if (IsMirror == true)
	{
		Target->SetHealth(Target->GetHealth() - (this->GetIntelligence() * 2.1 + ((this->GetIntelligence() * 2.1) * (Target->GetResistance() * 0.05))));
		IsMirror = false;
	}
}

void Wizard::MirrorImage()
{
	//mana cost 4
	this->SetMana(GetMana() - 3);

	//effect
	IsMirror = true;
}





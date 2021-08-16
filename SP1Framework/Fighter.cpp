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
}

Fighter::~Fighter()
{
}

//skills
void Fighter::Cleave(int SkillIndex, Class* Target1, Class* Target2, Class* Target3, Class* Target4)
{
	//mana cost 4
	this->SetMana(this->GetMana() - 4);

	if (Target1 != nullptr)
	{
		Target1->SetHealth(Target1->GetHealth() - (this->GetStrength() * 0.8 + Target1->GetDefence() * 0.5));
	}
	else if (Target2 != nullptr)
	{
		Target2->SetHealth(Target2->GetHealth() - (this->GetStrength() * 0.8 + Target2->GetDefence() * 0.5));
	}
	else if (Target3 != nullptr)
	{
		Target3->SetHealth(Target3->GetHealth() - (this->GetStrength() * 0.8 + Target3->GetDefence() * 0.5));
	}
	else if (Target4 != nullptr)
	{
		Target4->SetHealth(Target4->GetHealth() - (this->GetStrength() * 0.8 + Target4->GetDefence() * 0.5));
	}
}

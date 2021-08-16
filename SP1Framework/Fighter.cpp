#include "Fighter.h"

Fighter::Fighter()
{
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

//functions
void Fighter::SkillList(int SkillIndex, Class* Target)
{

	//enum Skill
	//{
	//	Cleave,
	//	Smash
	//};

	//switch (SkillIndex)
	//{
	//case Cleave:
	//	//mana cost 4
	//	this->SetMana(GetMana() - 4);

	//	//deal damage to all Classes in party
	//	for (int i = 0; i < 4; i++)
	//	{ 
	//		Target = Party->GetPartyClass(i);
	//		if (Target != nullptr)
	//		{
	//			Target->SetHealth(Target->GetHealth() - (this->GetStrength() * 0.6 + Target->GetDefence() * 0.5));
	//		}
	//	}
	//	break;

	//case Smash:
	//	//mana cost 3
	//	this->SetMana(GetMana() - 3);

	//	//deal amplified damage to a target
	//	Target = Party->GetPartyClass(PartyIndex);
	//	if (Target != nullptr)
	//	{
	//		Target->SetHealth(Target->GetHealth() - (this->GetStrength() * 1.3 + Target->GetDefence() * 0.5));
	//	}
	//}
}

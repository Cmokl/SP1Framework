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
void Fighter::SkillList(int SkillIndex, Party* Party, int PartyIndex)
{
	Class* Target;

	enum Skill
	{
		Cleave
	};

	switch (SkillIndex)
	{
	case Cleave:
		for (int i = 0; i < 4; i++)
		{ 
			Target = Party->GetPartyClass(i);
			if (Target != nullptr)
			{
				Target->SetHealth(Target->GetHealth() - (this->GetStrength() * 0.6 + Target->GetDefence() * 0.5));
			}
		}
	}
}

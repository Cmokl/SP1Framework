#include "Wizard.h"
Wizard::Wizard()
{
	this->SetHealth(25);
	this->SetMaxHealth(25);
	this->SetMana(40);
	this->SetMaxMana(40);
	this->SetStrength(8);
	this->SetIntelligence(16);
	this->SetFaith(13);
	this->SetSpeed(13);
	this->SetDefence(9);
	this->SetResistance(12);

	IsMirror = false;
	CastCount = 0;
}
Wizard::~Wizard()
{
}

//skills
void Wizard::MagicMissile(Class* Target[])
{
	int Targeted;

	//mana cost 3
	this->SetMana(GetMana() - 3);

	//effect
	srand(static_cast<unsigned int>(time(0)));
	
	for (int i = 0; i < 3; i++)
	{
		Targeted = rand() % 3; //random number from 0 to 3
		Target[Targeted]->SetHealth(Target[Targeted]->GetHealth() - (this->GetIntelligence() * 0.8 + ((this->GetIntelligence() * 0.8) * (Target[Targeted]->GetResistance() * 0.05))));
	}

	if (IsMirror == true)
	{
		srand(static_cast<unsigned int>(time(0)));
		for (int i = 0; i < 3; i++)
		{
			Targeted = rand() % 3; //random number from 0 to 3
			Target[Targeted]->SetHealth(Target[Targeted]->GetHealth() - (this->GetIntelligence() * 0.8 + ((this->GetIntelligence() * 0.8) * (Target[Targeted]->GetResistance() * 0.05))));
		}
		IsMirror = false;
	}
}

void Wizard::PyroBlast(Class* Target)
{
	CastCount++;
	if (CastCount == 0)
	{
		//mana cost 3
		this->SetMana(GetMana() - 3);
	}

	if (CastCount == 2)
	{
		//effect
		Target->SetHealth(Target->GetHealth() - (this->GetIntelligence() * 2.1 + ((this->GetIntelligence() * 2.1) * (Target->GetResistance() * 0.05))));

		if (IsMirror == true)
		{
			Target->SetHealth(Target->GetHealth() - (this->GetIntelligence() * 2.1 + ((this->GetIntelligence() * 2.1) * (Target->GetResistance() * 0.05))));
			IsMirror = false;
		}
		CastCount = 0;
	}
}

void Wizard::MirrorImage()
{
	//mana cost 2
	this->SetMana(GetMana() - 2);

	//effect
	IsMirror = true;
}

void Wizard::SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4])
{
	if (ListIndex == 0)
	{
		MagicMissile(TargetParty);
	}
	else if (ListIndex == 1)
	{
		PyroBlast(TargetParty[ClassIndex]);
	}
	else if (ListIndex == 2)
	{
		MirrorImage();
	}
}

int Wizard::SkillTargetType(int ListIndex)
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

std::string Wizard::SkillNameList(int ListIndex)
{
	if (ListIndex == 0)
	{
		return "Magic Missile";
	}
	else if (ListIndex == 1)
	{
		return "Pyro Blast";
	}
	else if (ListIndex == 2)
	{
		return "Mirror Image";
	}

	return std::string();
}

int Wizard::ManaCost(int ListIndex)
{
	if (ListIndex == 0)
	{
		return 3;
	}
	else if (ListIndex == 1)
	{
		return 2;
	}
	else if (ListIndex == 2)
	{
		return 2;
	}
	return 0;
}





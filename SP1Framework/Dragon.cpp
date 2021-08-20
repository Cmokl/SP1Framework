#include "Dragon.h"

Dragon::Dragon()
{
	this->SetHealth(55);
	this->SetMaxHealth(55);
	this->SetMana(40);
	this->SetStrength(16);
	this->SetIntelligence(10);
	this->SetFaith(8);
	this->SetSpeed(16);
	this->SetDefence(15);
	this->SetResistance(15);
	DragonsCallUsed = 0;
}
Dragon::~Dragon()
{
}
void Dragon::FireBreath(Class* target)
{
	target->SetHealth(target->GetHealth() - 10 + 10 * (0.1 * (target->GetResistance() / 10)));
	target->SetIsBurn(true);
	//to add the player burning for 3 turns
}

void Dragon::TalonTear(Class* target)
{
	target->SetHealth(target->GetHealth() - 9 + 9 * (0.1 * (target->GetDefence() / 10)));
}

void Dragon::FlameBurst(Class* target[4])
{
	for (int i = 0; i < 4; i++)
	{
		if (target[i] != nullptr)
		{
			target[i]->SetHealth(target[i]->GetHealth() - 9 + 9 * (0.1 * (target[i]->GetDefence() / 10)));
		}
	}
}

void Dragon::DragonsCall(Class* NewEnemy)
{
	if (DragonsCallUsed == 0 && (GetHealth() <= (GetMaxHealth() * 0.25)))
	{
		NewEnemy = new Oni;
		DragonsCallUsed = 1;
	}
}

void Dragon::SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4])
{
	if (ListIndex == 0)
	{
		FireBreath(TargetParty[ClassIndex]);
	}
	else if (ListIndex == 1)
	{
		TalonTear(TargetParty[ClassIndex]);
	}
	else if (ListIndex == 2)
	{
		FlameBurst(TargetParty);
	}
	else if (ListIndex == 2)
	{
		DragonsCall(TargetParty[ClassIndex]);
	}
}

std::string Dragon::SkillNameList(int ListIndex)
{
	if (ListIndex == 0)
	{
		return "Fire Breath";
	}
	else if (ListIndex == 1)
	{
		return "Talon Tear";
	}
	else if (ListIndex == 2)
	{
		return "Flame Burst";
	}
	else if (ListIndex == 3)
	{
		return "Dragons Call";
	}

	return "";
}


#include "Dragon.h"

Dragon::Dragon()
{
	this->SetName("Dragon");
	this->SetHealth(200);
	this->SetMaxHealth(200);
	this->SetStrength(16);
	this->SetIntelligence(16);
	this->SetFaith(8);
	this->SetSpeed(16);
	this->SetDefence(15);
	this->SetResistance(15);
}
Dragon::~Dragon()
{
}
void Dragon::FireBreath(Class* target)
{
	target->SetHealth(target->GetHealth() - this->GetIntelligence() * 0.9 + (this->GetIntelligence() * 1.0) * (0.01 * (target->GetResistance())));
	target->SetIsBurn(true);
	//to add the player burning for 3 turns
}

void Dragon::TalonTear(Class* target)
{
	target->SetHealth(target->GetHealth() - this->GetStrength() * 1.1 + (this->GetStrength() * 1.1) * (0.01 * (target->GetResistance())));
}

void Dragon::FlameBurst(Class* target[4])
{
	for (int i = 0; i < 4; i++)
	{
		if (target[i]->GetHealth() > 0)
		{
			target[i]->SetHealth(target[i]->GetHealth() - this->GetStrength() * 1.8 + (this->GetStrength() * 1.8) * (0.01 * (target[i]->GetResistance())));
		}
	}
}

void Dragon::DragonsCall(Class* OwnParty[4])
{
	OwnParty[1] = new Oni;
}

bool Dragon::SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4])
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
	else if (ListIndex == 3)
	{
		DragonsCall(TargetParty);
	}
	return true;
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


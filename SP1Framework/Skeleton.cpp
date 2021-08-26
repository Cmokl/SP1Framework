#include "Skeleton.h"

Skeleton::Skeleton()
{
	this->SetName("Skeleton");
	this->SetHealth(20);
	this->SetMaxHealth(20);
	this->SetMana(15);
	this->SetStrength(9);
	this->SetIntelligence(6);
	this->SetFaith(6);
	this->SetSpeed(13);
	this->SetDefence(12);
	this->SetResistance(10);
}
Skeleton::~Skeleton()
{
}
void Skeleton::Pierce(Class* target)
{
	if (target != nullptr)
	{
		target->SetHealth(target->GetHealth() - 6 + 6 * (0.1 * (target->GetDefence() / 10)));
	}
}
bool Skeleton::Reassemble()
{
	if (this->GetHealth() != this->GetMaxHealth())
	{
		for (int i = 0; i < 4; i++)
		{
			if (this->GetHealth() + 1 < this->GetMaxHealth())
			{
				this->SetHealth(this->GetHealth() + 1);
			}
		}
		return true;
	}
	return false;
}

//skill list
bool Skeleton::SkillList(int ListIndex, int ClassIndex, Class* TargetClass[])
{
	if (ListIndex == 0)
	{
		if (TargetClass[ClassIndex] != nullptr)
		{
			Pierce(TargetClass [ClassIndex] );
		}
	}
	else if (ListIndex == 1)
	{
		return Reassemble();
	}
	return true;
}

std::string Skeleton::SkillNameList(int ListIndex)
{
	if (ListIndex == 0)
	{
		return "Pierce";
	}
	else if (ListIndex == 1)
	{
		return "Reassemble";
	}

	return "";
}

#include "Skill.h"

Skill::Skill()
{
	ManaCost = 0;

	for (int i = 0; i < 4; i++)
	{
		Targets[i] = nullptr;
	}
}

Skill::Skill(int ManaCost)
{
	this->ManaCost = ManaCost;

	for (int i = 0; i < 4; i++)
	{
		Targets[i] = nullptr;
	}
}

Skill::Skill(int ManaCost, Party* Target)
{
	this->ManaCost = ManaCost;

	for (int i = 0; i < 4; i++)
	{
		Targets[i] = Target->GetPartyClass(i);
	}
}

Skill::~Skill()
{
}

void Skill::SetManaCost(int Mana)
{
	ManaCost = Mana;
}

int Skill::GetManaCost(void)
{
	return ManaCost;
}

Class* Skill::GetTarget(int TargetIndex)
{
	return Targets[TargetIndex];
}

void Skill::SetTarget(int TargetIndex, Class* Target)
{
	Targets[TargetIndex] = Target;
}

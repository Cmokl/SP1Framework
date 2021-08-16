#pragma once
#include "Class.h"
#include "Party.h"
class Skill
{
private:
	Class* Targets[4];
	int ManaCost;
public:
	Skill();
	Skill(int ManaCost);
	Skill(int ManaCost, Party* Target);
	~Skill();

	//getter/setters
	void SetManaCost(int Mana);
	int GetManaCost(void);
	Class* GetTarget(int TargetIndex);
	void SetTarget(int TargetIndex, Class* Target);
};
#pragma once
#include "Class.h"

class Fighter : public Class
{
private:
	bool IsBattleCry;
public:
	Fighter();
	~Fighter();

	//skills
	//deals damage to everyone in the enemy party
	void Cleave(Class* Target[4]);
	//deals heavy damage to one target
	void Smash(Class* Target);
	//increases the Fighter's strength by 10%
	void BattleCry(void);
	//Revert battlecry
	void BattleCryRevert();

	//skill list
	bool SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4]);
	int SkillTargetType(int ListIndex);
	std::string SkillNameList(int ListIndex);
	int ManaCost(int ListIndex);
};


#pragma once
#include "Class.h"

class Fighter : public Class
{
private:
public:
	Fighter();
	~Fighter();

	//skills
	//deals damage to everyone in the enemy party
	void Cleave(Class* Target[]);
	//deals heavy damage to one target
	void Smash(Class* Target);
	//increases the Fighter's strength by 10%
	void BattleCry(void);
	//Revert battlecry
	void BattleCryRevert();

	//skill list
	void SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4]);
	std::string SkillNameList(int ListIndex);
};


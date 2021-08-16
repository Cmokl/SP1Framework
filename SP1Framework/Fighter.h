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
	void Cleave(Class* Target1, Class* Target2, Class* Target3, Class* Target4);
	//deals heavy damage to one target
	void Smash(Class* Target);
	//increases the Fighter's strength by 10%
	void BattleCry(void);
	//Revert battlecry
	void BattleCryRevert(int OriginalStrength);
};


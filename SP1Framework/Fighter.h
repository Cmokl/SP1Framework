#pragma once
#include "Class.h"
#include "Party.h"

class Fighter : public Class
{
private:
	int OriginalStrength;
public:
	Fighter();
	~Fighter();

	//skills
	//deals damage to everyone in the enemy party
	void Cleave(Party* Target);
	//deals heavy damage to one target
	void Smash(Class* Target);
	//increases the Fighter's strength by 10%
	void BattleCry(void);
	//Revert battlecry
	void BattleCryRevert();
};


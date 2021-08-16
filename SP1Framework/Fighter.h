#pragma once
#include "Class.h"
class Fighter : public Class
{
private:

public:
	Fighter();
	~Fighter();

	//functions
	void SkillList(int SkillIndex, Party* Party, int PartyIndex);
};


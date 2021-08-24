#pragma once
#include "Class.h"
class EmptyClass : public Class
{
public:
	EmptyClass();
	~EmptyClass();

	void SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4]);
	std::string SkillNameList(int ListIndex);
};


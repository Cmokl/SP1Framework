#pragma once
#include "Class.h"
class Fighter : public Class
{
private:
public:
	Fighter();
	~Fighter();

	//skills
	void Cleave(int SkillIndex, Class* Target1, Class* Target2, Class* Target3, Class* Target4);
};


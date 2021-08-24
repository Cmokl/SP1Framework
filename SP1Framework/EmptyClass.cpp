#include "EmptyClass.h"

EmptyClass::EmptyClass()
{
}

EmptyClass::~EmptyClass()
{
}

bool EmptyClass::SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4])
{
    return true;
}

std::string EmptyClass::SkillNameList(int ListIndex)
{
    return std::string();
}

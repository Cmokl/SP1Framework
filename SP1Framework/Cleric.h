#pragma once
#include "Class.h"

class Cleric :
    public Class
{
public:
    Cleric();
    ~Cleric();

    //skills
    bool HolyRestoration(Class* target);
    bool Resurrection(Class* target);
    void Protection(Class* TargetParty[]);

    //skill list
    bool SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4]);
    int SkillTargetType(int ListIndex);
    std::string SkillNameList(int ListIndex);
    int ManaCost(int ListIndex);
};


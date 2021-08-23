#pragma once
#include "Class.h"

class Cleric :
    public Class
{
public:
    Cleric();
    ~Cleric();

    //skills
    void HolyRestoration(Class* target);
    void Resurrection(Class* target);
    void Protection(Class* TargetParty[]);

    //skill list
    void SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4]);
    int SkillTargetType(int ListIndex);
    std::string SkillNameList(int ListIndex);
    int ManaCost(int ListIndex);
};


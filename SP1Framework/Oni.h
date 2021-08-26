#pragma once
#include "Class.h"

class Oni :
    public Class
{
public:
    Oni();
    ~Oni();

    //skills
    void InfernalBlast(Class* Target);
    void SoulLock(Class* Target);

    //skill list
    bool SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4]);
    std::string SkillNameList(int ListIndex);
};


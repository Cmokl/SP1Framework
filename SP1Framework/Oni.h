#pragma once
#include "Class.h"

class Oni :
    public Class
{
public:
    Oni();
    ~Oni();

    //skills
    void InfenalBlast(Class* Target);
    void SoulLock(Class* Target);
    void RevertSoulLock(Class* Target);

    //skill list
    void SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4]);
    std::string SkillNameList(int ListIndex);
};


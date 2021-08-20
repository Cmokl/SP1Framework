#pragma once
#include "Class.h"

class Skeleton ://this is one of the weaker enemies
    public Class
{
public:
    Skeleton();
    ~Skeleton();

    //skills
    void Pierce(Class* target);
    void Reassemble();

    //skill list
    void SkillList(int ListIndex, int ClassIndex, Class* TargetClass[]);
    std::string SkillNameList(int ListIndex);
};

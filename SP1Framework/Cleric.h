#pragma once
#include "Class.h"
class Cleric :
    public Class
{
public:
    Cleric();
    ~Cleric();

    //functions
    void SkillList(int SkillIndex, Party* Party, int PartyIndex);
};


#pragma once
#include "Class.h"
#include"Party.h"
#include <stdlib.h>
#include <time.h>

class Wizard :
    public Class
{
private:
    //if IsMirror is true casts spells twice
    bool IsMirror;

    //int for cast timer
    int CastCount;
public:
    Wizard();
    ~Wizard();

    //skills
    //deals damage to 3 random targets
    void MagicMissile(Party* Target);
    //deals great damage to a target cast time of 2 in game logic
    void PyroBlast(Class* Target);
    //sets IsMirror to true
    void MirrorImage();

    //skill list
    void SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4]);
    std::string SkillNameList(int ListIndex);
};


#pragma once
#include "Class.h"
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
    void MagicMissile(Class* Target[]);
    //deals great damage to a target cast time of 2 in game logic
    void PyroBlast(Class* Target);
    //sets IsMirror to true
    bool MirrorImage();

    //skill list
    bool SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4]);
    int SkillTargetType(int ListIndex);
    std::string SkillNameList(int ListIndex);
    int ManaCost(int ListIndex);
};


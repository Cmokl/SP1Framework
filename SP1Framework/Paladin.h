#pragma once
#include "Class.h"

class Paladin :
    public Class
{
private:
    bool IsFlame;
    int GuardedIndex;
public:
    Paladin();
    ~Paladin();

    //attack override because of HolyFlame
    void Attack(Class* Target);

    //skills
    void Guard(Class* Party[]);//party is own party
    void RevertGuard(Class* Party[]);
    void InspiringAura(Class* Self[]);
    void RevertInspiringAura(Class* Self[]);

    //skill list
    bool SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4]);
    int SkillTargetType(int ListIndex);
    std::string SkillNameList(int ListIndex);
    int ManaCost(int ListIndex);
};


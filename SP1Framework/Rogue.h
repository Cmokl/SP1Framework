#pragma once
#include "Class.h"

class Rogue :
    public Class
{
private:
    bool IsStealth;
public:
    Rogue();
    ~Rogue();

    //getter
    bool GetIsStealth(void);
    void SetIsStealth(bool boolean);

    //rogue overrides attack because his attacks get stronger if stealthed
    void Attack(Class* Target);

    //skills
    bool Stealth(); //party here is rogue's own party
    bool CheapShot(Class* Target);
    bool Lacerate(Class* Target);

    //skill list
    bool SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4]);
    int SkillTargetType(int ListIndex);
    std::string SkillNameList(int ListIndex);
    int ManaCost(int ListIndex);
};
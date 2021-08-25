#pragma once
#include "Class.h"
#include "Oni.h"

class Dragon :
    public Class
{
private:
public:
    Dragon();
    ~Dragon();

    //skills
    void FireBreath(Class* target);
    void TalonTear(Class* target);
    void FlameBurst(Class* target[4]);
    void DragonsCall(Class* OwnParty[4]);//a pointer has to be created for the summoned Oni

    //skill list
    bool SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4]);
    std::string SkillNameList(int ListIndex);
};


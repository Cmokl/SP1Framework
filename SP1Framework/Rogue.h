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

    //rogue overrides attack because his attacks get stronger if stealthed
    void Attack(Class* Target);

    //skills
    void Stealth(void);
    void CheapShot(Class* Target);
    void Lacerate(Class* Target);
};
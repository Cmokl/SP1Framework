#pragma once
#include "Class.h"
#include "Party.h"

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

    //rogue overrides attack because his attacks get stronger if stealthed
    void Attack(Class* Target);

    //skills
    void Stealth(Party* Target); //party here is rogue's own party
    void CheapShot(Class* Target);
    void Lacerate(Class* Target);
};
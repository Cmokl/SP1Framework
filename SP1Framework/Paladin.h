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


};


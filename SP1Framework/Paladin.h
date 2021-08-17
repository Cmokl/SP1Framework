#pragma once
#include "Class.h"
#include "Party.h"

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
    void Guard(Class* Target, Party* Self);//party is own party
    void RevertGuard(Class* Target, Party* Self);
    void InspiringAura(Party* Self);
    void RevertInspiringAura(Party* Self);


};


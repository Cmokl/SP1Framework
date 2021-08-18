#pragma once
#include "Class.h"
class Oni :
    public Class
{
public:
    Oni();
    ~Oni();

    //skills
    void InfenalBlast(Class* Target);
    void SoulLock(Class* Target);
    void RevertSoulLock(Class* Target);
};


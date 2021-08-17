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

    void Attack(Class* Target); //Party index determines the target
    void Defend();

    //skills
};

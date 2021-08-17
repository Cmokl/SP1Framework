#pragma once
#include "Class.h"
#include "Oni.h"
class Dragon :
    public Class
{
private:
    int DragonsCallUsed;
public:
    Dragon();
    ~Dragon();
    void FireBreath(Class* target);
    void TalonTear(Class* target);
    void FlameBurst(Class* target1, Class* target2, Class* target3, Class* target4);
    void DragonsCall(Oni* NewEnemy);//a pointer has to be created for the summoned Oni
};


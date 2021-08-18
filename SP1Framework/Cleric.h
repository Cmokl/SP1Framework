#pragma once
#include "Class.h"
class Cleric :
    public Class
{
public:
    Cleric();
    ~Cleric();
    void HolyRestoration(Class* target);
    void Resurrection(Class* target);
    void Protection(Class* target1, Class* target2, Class* target3, Class* target4);
};


#pragma once
#include "Class.h"
class Medusa :
    public Class
{
public:
    Medusa();
    ~Medusa();
    void CursedGaze(Class* target);
    void SerpentBite(Class* target);
};

#pragma once
#include "Class.h"
class Ghoul :
    public Class
{
public:
    Ghoul();
    ~Ghoul();

    //skills
    void Devour(Class* Target);
    void Shapeshift(void);
    void RevertShapeshift(void);
};


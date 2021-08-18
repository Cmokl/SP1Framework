#pragma once
#include "Class.h"
class Skeleton ://this is one of the weaker enemies
    public Class
{
public:
    Skeleton();
    ~Skeleton();
    void Pierce(Class* target);
    void Reassemble();
};
#pragma once
#include "Class.h"
#include"Party.h"
#include <stdlib.h>
#include <time.h>

class Wizard :
    public Class
{
public:
    Wizard();
    ~Wizard();

    //skills
    void MagicMissile(Party* Target);

};


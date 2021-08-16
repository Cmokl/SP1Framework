#include "Movement.h"
#include <string>
#include <iostream>
#include "Position.h"

void movement(string move)
{

    if (move == "1")
    {
        if (s.get_y() > 2)
        {
            s.set_y(s.get_y() - 2);
        }
        else if (s.get_y() == 2)
        {
            s.set_y(s.get_y() - 1);
        }

    }
    else if (move == "2")
    {

        if (s.get_y() < 19)
        {
            s.set_y(s.get_y() + 2);
        }
        else if (s.get_y() == 19)
        {
            s.set_y(s.get_y() + 1);
        }


    }
    else if (move == "3")
    {
        if (s.get_x() > 2)
        {
            s.set_x(s.get_x() - 2);
        }
        else if (s.get_x() == 2)
        {
            s.set_x(s.get_x() - 1);
        }
    }
    else if (move == "4")
    {
        if (s.get_x() < 19)
        {
            s.set_x(s.get_x() + 2);
        }
        else if (s.get_x() == 19)
        {
            s.set_x(s.get_x() + 1);
        }
    }

}

void Movement::set_letter(char l)
{
}

char Movement::get_letter()
{
    return 0;
}

void Movement::movement(char move)
{
}

void Movement::movement(string move)
{
}

int Movement::get_x()
{
    return get_x();
}

void Movement::set_x(int x)
{
    set_x(x);
}

int Movement::get_y()
{
    return get_y();
}

void Movement::set_y(int y)
{
    set_y(y);
}

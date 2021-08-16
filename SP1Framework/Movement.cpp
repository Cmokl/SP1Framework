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

int Movement::get_x()
{
    return s.get_x();
}

void Movement::set_x(int x)
{
    s.set_x(x);
}

int Movement::get_y()
{
    return s.get_y();
}

void Movement::set_y(int y)
{
    s.set_y(y);
}

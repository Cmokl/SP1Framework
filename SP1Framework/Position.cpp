#include "Position.h"

int Position::setx()
{
	return player_x;
}

int Position::sety()
{
	return player_y;
}

void Position::getx(int current_x)
{
	player_x = current_x;
}

void Position::gety(int current_y)
{
	player_y = current_y;
}



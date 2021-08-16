#pragma once
class Position
{
private:
	int player_x;
	int player_y;
public:
	int setx();
	int sety();
	void getx(int current_x);
	void gety(int current_y);
	void movement(string move);
};





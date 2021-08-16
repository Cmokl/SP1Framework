#pragma once
class Movement
{
public:
    void set_letter(char l);
    char get_letter();
    void movement(char move);
    void movement(string move);
    int get_x();
    void set_x(int y);
    int get_y();
    void set_y(int y);

private:
    char L;
    Movement s;
};

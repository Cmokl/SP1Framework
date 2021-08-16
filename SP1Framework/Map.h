#pragma once
#include <string>
#include <iostream>

using namespace std;
class Map
{
public:
    char box[25][25];
    void Add(int H, int V, char p);

    void Draw();


    void ShoMap();
    Map()
    {

    }


private:




};

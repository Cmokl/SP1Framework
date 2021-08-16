#pragma once
#include <string>
#include <iostream>

using namespace std;
class Map
{
public:
    char box[21][21];
    void Add(int H, int V, char p);

    void Draw();


    void ShoMap();
    Map()
    {

    }


private:




};

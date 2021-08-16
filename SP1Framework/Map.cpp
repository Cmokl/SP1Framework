#include "Map.h"
using namespace std;

void Map::Add(int H, int V, char p)
{
    box[V][H] = p;

}

void Map::Draw()
{
    int A = 49;
    for (int i = 0; i < 21; i++)
    {
        for (int l = 0; l < 21; l++)
        {
            box[i][l] = ' ';
            if (l > 0 && i == 0)
            {
                box[i][l] = (A);
                /*A++;
                if (A > 57)
                {
                    A = 48;

                }*/
                A = '-';
            }
            if (i > 0 && l == 0)
            {
                box[i][l] = (A);
                /*A++;
                if (A > 57)
                {
                    A = 48;

                }*/
                A = '|';

            }
        }
    }
}


void Map::ShoMap()
{
    for (int i = 0; i < 21; i++)
    {
        for (int l = 0; l < 21; l++)
        {
            cout << box[i][l];
        }
        cout << endl;
    }
}

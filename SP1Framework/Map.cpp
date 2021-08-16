#include "Map.h"
using namespace std;

void Map::Add(int H, int V, char p)
{
    box[V][H] = p;

}

void Map::Draw()
{
    int A = 45;
    for (int i = 0; i < 25; i++)
    {
        for (int l = 0; l < 25; l++)
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
                A = 124;
                box[i][l] = (A);
                /*A++;
                if (A > 57)
                {
                    A = 48;

                }*/
                A = '|';
            }

            if (i > 0 && l == 24)
            {
                A = 124;
                box[i][l] = (A);
                /*A++;
                if (A > 57)
                {
                    A = 48;

                }*/
                A = '|';
            }

            if (l > 0 && i == 24)
            {
                A = '-';
                box[i][l] = (A);
                /*A++;
                if (A > 57)
                {
                    A = 48;

                }*/
            }
            box[24][24] = '|';
        }
    }
}


void Map::ShoMap()
{
    for (int i = 0; i < 25; i++)
    {
        for (int l = 0; l < 25; l++)
        {
            cout << box[i][l];
        }
        cout << endl;
    }
}
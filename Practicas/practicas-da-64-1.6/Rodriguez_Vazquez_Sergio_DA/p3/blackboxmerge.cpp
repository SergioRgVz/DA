#include <iostream>
#include "cronometro.h"
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#define N 10

using namespace std;
int main()
{
    int v[8];
    int w[8];
    for(int n = 1; n <= 8; n++)
    {
        v[n - 1] = n - 1; //Permutacion actual ordenada
        memcpy(w, v, sizeof v); //Vector ordenado
        do
        {
            if(!pruebaordena(v, w, n))
            {
                cout << "ERROR.\a" << endl;
                return 1;
            }
        }while(next_permutation(v, v + n)); //Siguiente permutacion
    }
    return 0;
}
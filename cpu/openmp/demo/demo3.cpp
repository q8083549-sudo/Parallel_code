#include<iostream>
#include<omp.h>
using namespace std;

int main()
{
    int x = 10;
    #pragma omp parallel firstprivate(x)
    {
        cout << x << endl;
    }

    #pragma  omp parallel for lastprivate(x)
    for(int i = 0; i < 4; i++)
    {
        x = i;
    }

    cout << x << endl;
}
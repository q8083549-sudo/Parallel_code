#include<iostream>
#include<omp.h>
#include<math.h>
using namespace std;

int main()
{
    int a[10], b[10], c[10];
    double d[10];
    for(int i = 0; i <10; i++)
    {
        a[i] = i;
        c[i] = i;
    }

    #pragma omp paralell
    {
        #pragma omp for nowait
        for(int i = 1; i < 10; i++)
        {
            b[i] = a[i-1] + a[i];
        }

        #pragma omp for nowait
        for(int i = 0; i < 10; i++)
        {
            d[i] = sqrt(c[i]);
        }
    }

    for(int i = 0; i< 10; i++)
    {
        cout << b[i] << " ---  " << d[i] << endl; 
    }

    return 0;
}
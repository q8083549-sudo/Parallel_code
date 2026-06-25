#include<iostream>
#include<omp.h>
using namespace std;

int main()
{
    int jlast, klast;
    #pragma omp parallel
    {
        #pragma omp for collapse(2) lastprivate(jlast, klast)
        for(int j = 0; j <= 2; j++)
        {
            for(int k = 0; k <= 3; k++)
            {
                jlast = j;
                klast = k;
            }
        }

        #pragma omp single
        cout << jlast << " -- " << klast << endl;
    }
    return 0;
}
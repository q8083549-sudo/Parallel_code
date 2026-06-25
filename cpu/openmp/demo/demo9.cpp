#include<iostream>
#include<omp.h>
using namespace std;

int main()
{
    int j, k;
    #pragma omp parallel num_threads(2)
    {
        #pragma omp for collapse(2) ordered private(j, k) schedule(static, 3)
        for(int j = 0; j<= 2; j++)
        {
            for(int k = 0; k <=3; k++)
            {
                #pragma omp ordered
                cout << omp_get_thread_num() << " " << j << " " << k << endl; 
            }
        }
    }
    return 0;
}
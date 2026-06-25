#include<iostream>
#include<omp.h>
using namespace std;

int main()
{
    omp_set_nested(1);
    omp_set_dynamic(0);
    #pragma omp parallel
    {
        #pragma omp parallel
        {
            #pragma omp single
            cout << "inner num_threads = " << omp_get_num_threads() << endl; 
        }
       
        #pragma omp barrier
        omp_set_nested(0);

        #pragma omp parallel
        {
            #pragma omp single
            cout << "inner num_threads = " << omp_get_num_threads() << endl;
        }

        #pragma omp barrier
        #pragma omp single
        {
            cout << "outer num_threads = " << omp_get_num_threads() << endl;
        }
    }

    return 0;
}
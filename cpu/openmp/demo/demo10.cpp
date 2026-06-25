#include<iostream>
#include<omp.h>
using namespace std;

#define NT 4

int main()
{
    int section_count = 0;
    omp_set_dynamic(0);
    omp_set_num_threads(NT);

    #pragma omp parallel
    {
        #pragma omp sections firstprivate(section_count)
        {
            #pragma omp section
            {
                section_count++;
                cout << section_count << " " << omp_get_thread_num() <<endl;
            }

            #pragma omp section
            {
                section_count++;
                cout << section_count << " " << omp_get_thread_num() <<endl;
            }
        }
    }
    return 0;
}
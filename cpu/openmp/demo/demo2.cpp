#include<iostream>
#include<omp.h>
using namespace std;

int main()
{
    int a[4], b[4], c[4];
    #pragma omp parallel for
    for(int i = 0; i < 4; i++)
    {
        a[i] = i;
        b[i] = i;
    }
    #pragma omp parallel for
    for(int i = 0; i < 4; i++)
    {
        int tid = omp_get_thread_num();
        c[i] = a[i] + b[i];
        cout << "c[" << i << "] " << "set by thread " << tid << endl;
    }
    for(int i = 0; i < 4; i++)
    {
        cout << c[i] << " ";
    }
    cout << endl;
    return 0;
}
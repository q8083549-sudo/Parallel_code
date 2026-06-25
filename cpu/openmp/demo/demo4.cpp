#include<iostream>
#include<omp.h>
using namespace std;

void subdomain(double *x, int istart, int ipoints)
{
    for(int i = 0; i < ipoints; i++)
    {
        x[i+istart] = 123.456;
    }
}

void sub(double *x, int npoints)
{
    int iam, nt, istart, ipoints;
    #pragma omp parallel default(shared) private(iam, nt, istart, ipoints)
    {
        iam = omp_get_thread_num();   //获取线程编号
        nt = omp_get_num_threads();   //获取线程数量
        ipoints = npoints / nt;
        istart = iam * ipoints;
        if(iam == nt-1) ipoints = npoints - istart;
        subdomain(x, istart, ipoints);
    }
}

int main()
{
    double array[10000];
    sub(array, 10000);
    for(int i = 0; i < 10000; i++)
    {
        cout << array[i] << " " ;
        if((i+1) % 50 == 0) cout << endl;
    }
    return 0;
}
#include<iostream>
#include<omp.h>
using namespace std;

#define N 1000

int main()
{
    int nteams_reqiured = 2;
    int max_thrds, tm_id;
    float sp_x[N], sp_y[N], sp_a = 0.0001e0;
    double dp_x[N], dp_y[N], dp_a = 0.001e0;

    #pragma omp teams num_teams(nteams_reqiured)
    {
        tm_id = omp_get_team_num();
        if(omp_get_num_teams() != 2)
        {
            cout << "teams reqiured 2 " << endl;
            exit(0);
        }
        if(tm_id == 0)
        {
            #pragma omp parallel
            {
                #pragma omp for
                for(int i = 0; i < N; i++)
                {
                    sp_x[i] = i * 0.0001;
                    sp_y[i] = i;
                }
                #pragma omp simd simdlen(8)
                for(int i = 0; i < N; i++)
                {
                    sp_x[i] = sp_a * sp_x[i] + sp_y[i];
                }
            }
        }
        if(tm_id == 1)
        {
            #pragma omp parallel
            {
                #pragma omp for
                for(int i = 0; i < N; i++)
                {
                    dp_x[i] = i * 0.0001;
                    dp_y[i] = i;
                }
                #pragma omp simd simdlen(4)
                for(int i = 0; i < N; i++)
                {
                    dp_x[i] = dp_a * dp_x[i] + dp_y[i];
                }
            }
        }
    }
    cout << " sp: " << sp_x[N-1] << " dp: " << dp_x[N-1] << endl;
    cout << " sp: " << sp_x[N/2] << " dp: " << dp_x[N/2] << endl;
    return 0;
}
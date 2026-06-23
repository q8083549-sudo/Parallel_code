#include<iostream>
#include"mpi.h"
#include<iomanip>
#include<algorithm>
using namespace std;

int main(int argc, char **argv)
{
    int const totalsize = 16;
    int const innersize = totalsize - 2;
    int const steps = 10;

    int myid, numprocs;
    int tag1, tag2;
    int left, right;
    int activate_id, activate_procs;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    activate_procs = min(numprocs, innersize);
    int color = (myid < activate_procs) ? 0 : MPI_UNDEFINED;

    MPI_Comm active_comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, myid, &active_comm);

    if(color == MPI_UNDEFINED)
    {
        MPI_Finalize();
        return 0;
    }

    MPI_Comm_rank(active_comm, &activate_id);
    MPI_Comm_size(active_comm, &activate_procs);

    int base = totalsize / activate_procs;
    int rem  = totalsize % activate_procs;
    int const mysize = base + (activate_id < rem ? 1 : 0);
    double a[mysize+2][totalsize], b[mysize+2][totalsize];

    //init
    for(int i = 0; i < mysize+2; i++)
    {
        for(int j = 0; j < totalsize; j++)
        {
            a[i][j] = 0;
            b[i][j] = 0;
        }
    }

    if(activate_id == 0)
    {
        for(int j = 0; j < totalsize; j++)
        {
            a[0][j] = 8.0;
            a[1][j] = 8.0;
            b[0][j] = 8.0;
            b[1][j] = 8.0;
        }
    }

    if(activate_id == activate_procs-1)
    {
        for(int j = 0; j < totalsize; j++)
        {
            a[mysize][j] = 8.0;
            a[mysize+1][j] = 8.0;
            b[mysize][j] = 8.0;
            b[mysize+1][j] = 8.0;
        }
    }

    for(int i = 0; i < mysize+2; i++)
    {
        a[i][0] = 8.0;
        a[i][totalsize-1] = 8.0;
        b[i][0] = 8.0;
        b[i][totalsize-1] = 8.0;
    }

    //  for(int id = 0; id < activate_procs; id++)
    // {
    //     if(id == activate_id)
    //     {
    //         cout << activate_id << endl;
    //         for(int i = 0; i < mysize + 2; i++)
    //             {
    //                 for(int j = 0; j < totalsize; j++)
    //                     {
    //                        cout << fixed << setprecision(2) << a[i][j] << " ";
    //                     }
    //                     cout << endl;
    //             } 
    //     }
    //     MPI_Barrier(active_comm);
    // }

    if(activate_id > 0) left = activate_id - 1;
    else left = MPI_PROC_NULL;
    if(activate_id < activate_procs-1) right = activate_id + 1;
    else right = MPI_PROC_NULL;
    
    int begin = 1;
    int end = mysize;
    if(activate_id == 0) begin = 2;
    if(activate_id == activate_procs-1) end = mysize - 1;

    tag1 = 3;
    tag2 = 4;

    //
    for(int n = 0; n < steps; n++)
    {
        MPI_Sendrecv(&a[mysize][0], totalsize, MPI_DOUBLE, right, tag1, 
        &a[0][0], totalsize, MPI_DOUBLE, left, tag1, active_comm, &status);
        MPI_Sendrecv(&a[1][0], totalsize, MPI_DOUBLE, left, tag2, 
        &a[mysize+1][0], totalsize, MPI_DOUBLE, right, tag2, active_comm, &status);

        if(begin <= end)
        {
            for(int i = begin; i <= end; i++)
            {
                for (int  j = 1; j < totalsize-1; j++)
                {
                    b[i][j] = (a[i-1][j] + a[i+1][j] + a[i][j-1] + a[i][j+1]) * 0.25;
                }
                
            }
        }

        for(int i = 0; i < mysize+2; i++)
        {
            for(int j = 0; j < totalsize; j++)
            {
                a[i][j] = b[i][j];
            }
        }
    }

    
     for(int id = 0; id < activate_procs; id++)
    {
        if(id == activate_id)
        {
            cout << activate_id << endl;
            for(int i = 0; i < mysize + 2; i++)
                {
                    for(int j = 0; j < totalsize; j++)
                        {
                           cout << fixed << setprecision(2) << a[i][j] << " ";
                        }
                        cout << endl;
                } 
        }
        MPI_Barrier(active_comm);
    }

    MPI_Finalize();
    return 0;
}
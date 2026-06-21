#include<iostream>
#include"mpi.h"
#include<iomanip>
using namespace std;

int main(int argc, char **argv)
{
    int const totalsize = 16;
    int const mysize = totalsize / 4;
    int const steps = 10;

    int myid, numprocs;
    double a[mysize+2][totalsize], b[mysize+2][totalsize];
    int begin_col, end_col;
    int left, right, tag1, tag2;
    MPI_Request req[4];
    MPI_Status status[4];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    cout << "Process " << myid << " is alive of " << numprocs << endl;
    
    //Init
    for(int i = 0; i < mysize+2; i++)
    {
        for(int j = 0; j < totalsize; j++)
        {
            a[i][j] = 0.0;
        }
    }

    for(int i = 0; i < totalsize; i++)
    {
        a[0][i] = 8.0;
        a[mysize+1][i] = 8.0;
        b[0][i] = 8.0;
        b[mysize+1][i] = 8.0;
    }

    if(myid == 0)
    {
        for(int i = 0; i < totalsize; i++)
        {
            a[1][i] = 8.0;
            b[1][i] = 8.0;
        }
    }

    if(myid == 3)
    {
        for(int i = 0; i < totalsize; i++)
        {
            a[mysize][i] = 8.0;
            b[mysize][i] = 8.0;
        }
    }

    for(int i = 0; i < mysize+2; i++)
    {
        a[i][0] = 8.0;
        a[i][totalsize-1] = 8.0;
        b[i][0] = 8.0;
        b[i][totalsize-1] = 8.0;
    }
    

    tag1 = 3;
    tag2 = 4;

    if(myid > 0)
    {
        left = myid - 1;
    }
    else
    {
        left = MPI_PROC_NULL;
    }

    if(myid < 3)
    {
        right = myid + 1;
    }
    else
    {
        right = MPI_PROC_NULL;
    }

    begin_col = 1;
    end_col = mysize;
    if(myid==0) begin_col = 2;
    if(myid==3) end_col = mysize - 1;

    for(int n = 0; n < steps; n++)
    {
        
        for(int i = 1; i < totalsize-1; i++)
        {
            b[begin_col][i] = (a[begin_col-1][i] + a[begin_col+1][i] + a[begin_col][i-1] + a[begin_col][i+1]) * 0.25;
            b[end_col][i] = (a[end_col+1][i] + a[end_col-1][i] + a[end_col][i+1] + a[end_col][i-1]) * 0.25;
        }

        MPI_Isend(&b[end_col][0], totalsize, MPI_DOUBLE, right, tag1, MPI_COMM_WORLD, &req[0]);
        MPI_Isend(&b[begin_col][0], totalsize, MPI_DOUBLE, left, tag2, MPI_COMM_WORLD, &req[1]);
        MPI_Irecv(&a[0][0], totalsize, MPI_DOUBLE, left, tag1, MPI_COMM_WORLD, &req[2]);
        MPI_Irecv(&a[mysize+1][0], totalsize, MPI_DOUBLE, right, tag2, MPI_COMM_WORLD, &req[3]);

        for(int i = begin_col+1; i < end_col; i++)
        {
            for(int j = 1; j < totalsize - 1; j++)
            {
                b[i][j] = (a[i][j+1] + a[i][j-1] + a[i+1][j] + a[i-1][j]) * 0.25;
            }
        }

        for(int i = begin_col; i < end_col+1; i++)
        {
            for(int j = 1; j < totalsize - 1; j++)
            {
                a[i][j] = b[i][j];
            }
        }

        for(int i = 0; i < 4; i++)
        {
            MPI_Wait(&req[i], &status[i]);
        }
        
    }

    for(int id = 0; id < numprocs; id++)
    {
        if(id == myid)
        {
            cout << id << endl;
            for(int i = 0; i < mysize + 2; i++)
                {
                    for(int j = 0; j < totalsize; j++)
                        {
                           cout << fixed << setprecision(2) << a[i][j] << " ";
                        }
                        cout << endl;
                } 
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    

    MPI_Finalize();

    return 0;
}
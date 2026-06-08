#include<iostream>
#include"mpi.h"
using namespace std;

#define SIZE 10
static int src = 0;
static int dest = 1;

int main(int argc, char **argv)
{
    int rank;
    int act_size = 0;
    int flag, rval, np;
    int buffer[SIZE];
    MPI_Status status, status1, status2;
    int count1, count2;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    if(np != 2)
    {
        cerr << "*** This program uses exactly 2 processes! ***" << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    act_size = 5;

    if(rank == src)
    {
        act_size = 1;
        MPI_Ssend(buffer, act_size, MPI_INT, dest, 1, MPI_COMM_WORLD);
        cout << "MPI_Ssend "<< act_size << "data,tag=1" << endl;
        act_size = 4;
        MPI_Ssend(buffer, act_size, MPI_INT, dest, 2, MPI_COMM_WORLD);
        cout << "MPI_Ssend "<< act_size << "data,tag=2" << endl;
    }
    else if(rank == dest)
    {
        MPI_Recv(buffer, act_size, MPI_INT, src, 1, MPI_COMM_WORLD, &status1);
        MPI_Recv(buffer, act_size, MPI_INT, src, 2, MPI_COMM_WORLD, &status2);
        MPI_Get_count(&status1, MPI_INT, &count1);
        cout << "receve " << count1 << "data," << "tag=" << status1.MPI_TAG << endl;
        MPI_Get_count(&status2, MPI_INT, &count2);
        cout << "receve " << count2 << "data," << "tag=" << status2.MPI_TAG << endl;
    }
    MPI_Finalize();
    return 0;
}
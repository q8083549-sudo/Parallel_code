#include<iostream>
#include"mpi.h"
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
    MPI_Request req[4];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    int color = (numprocs <= innersize) ? 0 : MPI_UNDEFINED;

    return 0;
}
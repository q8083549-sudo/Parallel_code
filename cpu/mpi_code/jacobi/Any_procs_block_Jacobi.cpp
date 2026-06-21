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

    MPI_Status status;
    MPI_Request req[4];

    return 0;
}
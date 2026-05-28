#include<iostream>
#include"mpi.h"
using namespace std;

#define SIZE 6
static int src = 0;
static int dest = 1; 

void Generate_data(double *buffer, int buff_size)
{
    for(int i = 0; i < buff_size; i++)
    {
        buffer[i] = (double)i + 1;
    }
}

void Buffered_Test_Send(double *buffer, int buff_size)
{

}

int main(int argc, char **argv)
{
    int rank;
    double buffer[SIZE], *tembuffer, *tepbuf;
    int tsize, bsize;
    char Current_Test = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == src)
    {
        Generate_data(buffer, SIZE);
        MPI_Pack_size(SIZE, MPI_DOUBLE, MPI_COMM_WORLD, &bsize);
        tembuffer = (double *) malloc(bsize + 2*MPI_BSEND_OVERHEAD);
        if(!tembuffer)
        {
            cerr << "Could not allocate bsend buffer of size " << bsize << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        MPI_Buffer_attach(tembuffer, 2*MPI_BSEND_OVERHEAD);
        Buffered_Test_Send(buffer, SIZE);
        

    }
    return 0; 
}
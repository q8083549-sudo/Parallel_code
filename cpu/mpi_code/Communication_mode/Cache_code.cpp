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
        buffer[i] = double(i) + 1;
    }
}

void Buffer_test_send(double *buffer, int buff_size)
{
    void *bbuffer;
    int size;
    cout << "buffered send message of "  << buff_size-1 << "data" << endl;
    for(int i = 0; i < buff_size-1; i++)
    {
        cout << buffer[i] << endl;
    }

    MPI_Bsend(buffer, buff_size-1, MPI_DOUBLE, dest, 2000, MPI_COMM_WORLD);
    buffer += buff_size - 1;
    cout << "buffered send message of one data" <<endl;
    cout << *buffer <<endl;

    MPI_Bsend(buffer, 1, MPI_DOUBLE, dest, 2000, MPI_COMM_WORLD);
    MPI_Buffer_detach(&bbuffer, &size);
    MPI_Buffer_attach(bbuffer, size);
}

void Normal_test_recv(double *buffer, int buff_size)
{
    MPI_Status status;
    double *b;
    b = buffer;
    MPI_Recv(b, buff_size-1, MPI_DOUBLE, src, 2000, MPI_COMM_WORLD, &status);
    cout << "standard receive a message of " << buff_size-1 << " data" << endl;
    for(int i = 0; i < buff_size-1; i++)
    {
        cout << "buf" << i << "=" << b[i] << endl;
    }
    b += buff_size - 1;
    MPI_Recv(b, 1, MPI_DOUBLE, src, 2000, MPI_COMM_WORLD, &status);
    cout << "standard receive a message of one data" << endl;
    cout << *b << endl;
}

int main(int argc, char **argv)
{
    int rank;
    double buffer[SIZE], *tmpbuffer, *tmpbuf;
    int tsize, bsize;
    char *Current_test = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == src)
    {
        Generate_data(buffer, SIZE);   // genearte send_data
        MPI_Pack_size(SIZE, MPI_DOUBLE, MPI_COMM_WORLD, &bsize); //calculate the space of the data to be send
        tmpbuffer = (double *) malloc(bsize + MPI_BSEND_OVERHEAD*2); 

        if(!tmpbuffer)
        {
            cerr << "Could not allocate bsend buffer of size:" << bsize << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        MPI_Buffer_attach(tmpbuffer, bsize + 2*MPI_BSEND_OVERHEAD);

        Buffer_test_send(buffer, SIZE);
    }
    else if(rank == dest)
    {
        Normal_test_recv(buffer, SIZE);
    }
    MPI_Finalize();
    return 0;
}
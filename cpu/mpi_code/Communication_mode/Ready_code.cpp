#include<iostream>
#include"mpi.h"
using namespace std;

void test_rsend()
{
    int const TEST_SIZE = 2000;
    int prev, next, count, tag, rank, size;
    MPI_Request request[2], indecise[2];
    MPI_Status status;
    double send_buff[TEST_SIZE], recv_buff[TEST_SIZE];
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size != 2)
    {
        cerr << "This test requires exactly 2 processes" << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    next = rank + 1;
    if(next>=size) next = 0;
    prev = rank - 1;
    if(prev<0) prev = size - 1;

    if(rank == 0)
    {
        cout << "Rsend test" << endl;
    }

    tag = 1456;
    count = TEST_SIZE / 3;

    if(rank == 0)
    {
        MPI_Recv(MPI_BOTTOM, 0, MPI_INT, next, tag, MPI_COMM_WORLD, &status);
        cout << "Process " << rank << " post ready send" << endl;
        MPI_Rsend(send_buff, count, MPI_DOUBLE, next, tag, MPI_COMM_WORLD);
    }
    else
    {
        cout << "Process " << rank << "post a receive call" << endl;
        MPI_Irecv(recv_buff, TEST_SIZE, MPI_DOUBLE, prev, tag, MPI_COMM_WORLD, &request[0]);
        MPI_Send(MPI_BOTTOM, 0, MPI_INT, prev, tag, MPI_COMM_WORLD);
        MPI_Wait(&request[0], &status);
        cout << "Process " << rank << " Receive Rsend message from " << status.MPI_SOURCE << endl;
    }

}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    test_rsend();
    MPI_Finalize();   
    return 0;
}
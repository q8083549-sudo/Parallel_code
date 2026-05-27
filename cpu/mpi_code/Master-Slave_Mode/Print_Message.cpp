#include<iostream>
#include<sstream>
#include<string>
#include<cstring>
#include"mpi.h"
using namespace std;

int master_io(void);
int slave_io(void);

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0)
    {
        master_io();
    }
    else
    {
        slave_io();
    }
    MPI_Finalize();
    return 0;
}

#define MSG_EXIT 1
#define MSG_PRINT_ORDERED 2
#define MSG_PRINT_UNORDERED 3

int master_io(void)
{
    int size, nslave, firstmsg;
    char buf[256], buf2[256];
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    nslave = size - 1;

    while(nslave > 0)
    {
        MPI_Recv(buf, 256, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        switch (status.MPI_TAG)
        {
            case MSG_EXIT:
                nslave--;
                break;
            case MSG_PRINT_UNORDERED:
                cout << buf << endl;
                break;
            case MSG_PRINT_ORDERED:
                firstmsg = status.MPI_SOURCE;
                for(int i = 1; i < size; i++)
                {
                    if(i == firstmsg)
                    {
                        cout << buf << endl;
                    }
                    else
                    {
                        MPI_Recv(buf2, 256, MPI_CHAR, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                        cout << buf2 << endl;
                    }
                }
                break;
        }
      
    }
    return 0;
}

int slave_io(void)
{
    int rank;
    ostringstream oss;
    string buf;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    oss << "Hello from slave " << rank << " ordered print\n";
    buf = oss.str();
    MPI_Send(buf.c_str(), buf.size()+1, MPI_CHAR, 0, MSG_PRINT_ORDERED, MPI_COMM_WORLD);

    oss.str(""); oss.clear();  // 清空流
    oss << "Goodbye from slave " << rank << " ordered print\n";
    buf = oss.str();
    MPI_Send(buf.c_str(), buf.size()+1, MPI_CHAR, 0, MSG_PRINT_ORDERED, MPI_COMM_WORLD);

    oss.str(""); oss.clear();
    oss << "I'm exiting " << rank << " unordered print\n";
    buf = oss.str();
    MPI_Send(buf.c_str(), buf.size()+1, MPI_CHAR, 0, MSG_PRINT_UNORDERED, MPI_COMM_WORLD);

    MPI_Send(NULL, 0, MPI_CHAR, 0, MSG_EXIT, MPI_COMM_WORLD);    
    return 0;
}

#include <stdio.h>
#include <string.h>
#include "mpi.h"

int master_io(void);
int slave_io(void);

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        master_io(); /*进程0作为主进程*/
    else
        slave_io();  /*其它进程作为从进程*/

    MPI_Finalize();
    return 0;
}

#define MSG_EXIT 1
#define MSG_PRINT_ORDERED 2   /*定义按续打印标识*/
#define MSG_PRINT_UNORDERED 3 /*定义乱续打印标识*/

/* 主进程部分 */
int master_io(void)
{
    int i, size, nslave, firstmsg;
    char buf[256], buf2[256];
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    nslave = size - 1;

    while (nslave > 0) {
        MPI_Recv(buf, 256, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);

        switch (status.MPI_TAG) {
        case MSG_EXIT:
            nslave--;
            break;
        case MSG_PRINT_UNORDERED:
            fputs(buf, stdout);
            break;
        case MSG_PRINT_ORDERED:
            firstmsg = status.MPI_SOURCE;
            for (i = 1; i < size; i++) {
                if (i == firstmsg)
                    fputs(buf, stdout);
                else {
                    MPI_Recv(buf2, 256, MPI_CHAR, i, MSG_PRINT_ORDERED,
                             MPI_COMM_WORLD, &status);
                    fputs(buf2, stdout);
                }
            }
            break;
        }
    }
    return 0;
}

/* 从进程部分 */
int slave_io(void)
{
    char buf[256];
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    sprintf(buf, "Hello from slave %d ordered print\n", rank);
    MPI_Send(buf, strlen(buf) + 1, MPI_CHAR, 0, MSG_PRINT_ORDERED, MPI_COMM_WORLD);

    sprintf(buf, "Goodbye from slave %d ordered print\n", rank);
    MPI_Send(buf, strlen(buf) + 1, MPI_CHAR, 0, MSG_PRINT_ORDERED, MPI_COMM_WORLD);

    sprintf(buf, "I'm exiting (%d), unordered print\n", rank);
    MPI_Send(buf, strlen(buf) + 1, MPI_CHAR, 0, MSG_PRINT_UNORDERED, MPI_COMM_WORLD);

    MPI_Send(NULL, 0, MPI_CHAR, 0, MSG_EXIT, MPI_COMM_WORLD);

    return 0;
}

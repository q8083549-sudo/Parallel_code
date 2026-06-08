program rsendtest
    use mpi
    implicit none

    integer :: ierr

    call MPI_Init(ierr)
    call test_rsend()
    call MPI_Finalize(ierr)

end program rsendtest


subroutine test_rsend()
    use mpi
    implicit none

    integer, parameter :: TEST_SIZE = 2000
    integer :: ierr, prev, next, count, tag, rank, size
    integer :: requests(2), indices(2)
    integer :: status(MPI_STATUS_SIZE), statuses(MPI_STATUS_SIZE,2)
    real :: send_buf(TEST_SIZE), recv_buf(TEST_SIZE)

    call MPI_Comm_rank(MPI_COMM_WORLD, rank, ierr)
    call MPI_Comm_size(MPI_COMM_WORLD, size, ierr)

    if (size /= 2) then
        print *, 'This test requires exactly 2 processes'
        call MPI_Abort(MPI_COMM_WORLD, 1, ierr)
    end if

    next = rank + 1
    if (next >= size) next = 0
    prev = rank - 1
    if (prev < 0) prev = size - 1

    if (rank == 0) then
        print *, "Rsend Test"
    end if

    tag = 1456
    count = TEST_SIZE / 3

    if (rank == 0) then
        ! 进程0先执行一个阻塞接收（接收0个数据）
        call MPI_Recv(MPI_BOTTOM, 0, MPI_INTEGER, next, tag, MPI_COMM_WORLD, status, ierr)

        print *, "Process ", rank, " post Ready send"
        call MPI_Rsend(send_buf, count, MPI_REAL, next, tag, MPI_COMM_WORLD, ierr)

    else
        print *, "Process ", rank, " post a receive call"
        call MPI_Irecv(recv_buf, TEST_SIZE, MPI_REAL, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, requests(1), ierr)

        ! 发送一个空消息，保证就绪发送在接收之后
        call MPI_Send(MPI_BOTTOM, 0, MPI_INTEGER, next, tag, MPI_COMM_WORLD, ierr)

        call MPI_Wait(requests(1), status, ierr)

        print *, "Process ", rank, " Receive Rsend message from ", status(MPI_SOURCE)
    end if

end subroutine test_rsend

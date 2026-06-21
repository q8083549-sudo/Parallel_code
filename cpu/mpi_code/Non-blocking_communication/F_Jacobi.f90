program main
  use mpi
  implicit none

  integer, parameter :: totalsize = 16
  integer, parameter :: mysize = totalsize / 4
  integer, parameter :: steps = 10

  integer :: n, myid, numprocs, i, j, rc, ierr
  real, dimension(totalsize, mysize+2) :: a, b
  integer :: begin_col, end_col
  integer :: left, right, tag1, tag2
  integer :: status(MPI_STATUS_SIZE,4)
  integer :: req(4)

  call MPI_Init(ierr)
  call MPI_Comm_rank(MPI_COMM_WORLD, myid, ierr)
  call MPI_Comm_size(MPI_COMM_WORLD, numprocs, ierr)

  print *, "Process ", myid, " of ", numprocs, " is alive"

  ! 数组赋初值
  do j = 1, mysize+2
     do i = 1, totalsize
        a(i,j) = 0.0
     end do
  end do

  do i = 1, totalsize
     a(i,1) = 8.0
     a(i,mysize+2) = 8.0
  end do

  if (myid == 0) then
     do i = 1, totalsize
        a(i,2) = 8.0
     end do
  end if

  if (myid == 3) then
     do i = 1, totalsize
        a(i,mysize+1) = 8.0
     end do
  end if

  do i = 1, mysize+2
     a(1,i) = 8.0
     a(totalsize,i) = 8.0
  end do

  tag1 = 3
  tag2 = 4

  ! 计算当前进程的左右相邻进程
  if (myid > 0) then
     left = myid - 1
  else
     left = MPI_PROC_NULL
  end if

  if (myid < 3) then
     right = myid + 1
  else
     right = MPI_PROC_NULL
  end if

  ! 设置每一块迭代的开始列和终止列
  begin_col = 2
  end_col = mysize + 1
  if (myid == 0) begin_col = 3
  if (myid == 3) end_col = mysize

  ! 执行迭代
  do n = 1, steps

     ! 先计算需要通信的边界数据
     do i = 2, totalsize-1
        b(i,begin_col) = (a(i,begin_col+1) + a(i,begin_col-1) + &
                          a(i+1,begin_col) + a(i-1,begin_col)) * 0.25
        b(i,end_col)   = (a(i,end_col+1) + a(i,end_col-1) + &
                          a(i+1,end_col) + a(i-1,end_col)) * 0.25
     end do

     ! 执行非阻塞通信
     call MPI_Isend(b(1,end_col), totalsize, MPI_REAL, right, tag1, &
                    MPI_COMM_WORLD, req(1), ierr)
     call MPI_Isend(b(1,begin_col), totalsize, MPI_REAL, left, tag2, &
                    MPI_COMM_WORLD, req(2), ierr)
     call MPI_Irecv(a(1,1), totalsize, MPI_REAL, left, tag1, &
                    MPI_COMM_WORLD, req(3), ierr)
     call MPI_Irecv(a(1,mysize+2), totalsize, MPI_REAL, right, tag2, &
                    MPI_COMM_WORLD, req(4), ierr)

     ! 计算剩余的部分
     do j = begin_col+1, end_col-1
        do i = 2, totalsize-1
           b(i,j) = (a(i,j+1) + a(i,j-1) + a(i+1,j) + a(i-1,j)) * 0.25
        end do
     end do

     ! 更新数组
     do j = begin_col, end_col
        do i = 2, totalsize-1
           a(i,j) = b(i,j)
        end do
     end do

     ! 完成非阻塞通信
     do i = 1, 4
        call MPI_Wait(req(i), status(1,i), ierr)
     end do

  end do

  ! 打印迭代后的结果
  do i = 2, totalsize-1
     print *, myid, (a(i,j), j=begin_col,end_col)
  end do

  call MPI_Finalize(rc)

end program main

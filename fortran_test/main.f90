program QuickSortFromFile
    implicit none
    integer, allocatable :: arr(:)
    integer :: count, i, io_status
    real :: start_time, end_time

    open(unit=10, file='text.txt', status='old', action='read')

    count = 0
    do
        read(10, *, iostat=io_status)
        if (io_status /= 0) exit
        count = count + 1
    end do

    rewind(10)

    allocate(arr(count), stat=io_status)
    if (io_status /= 0) then
        print *, "Ошибка: не удалось выделить память!"
        stop
    end if

    do i = 1, count
        read(10, *, iostat=io_status) arr(i)
        if (io_status /= 0) exit
    end do
    close(10)

    ! Сортировка
    call cpu_time(start_time)
    call quicksort(arr, 1, count)
    call cpu_time(end_time)

    print *, "Время сортировки (сек):", end_time - start_time

    open(unit=20, file='fortran_test/fortran_sorted.txt', status='replace', action='write')
    do i = 1, count
        write(20, '(I10)') arr(i)
    end do
    close(20)

    print *, "Отсортированный файл создан!"
    deallocate(arr)

contains
    recursive subroutine quicksort(A, left, right)
        integer, intent(inout) :: A(:)
        integer, intent(in) :: left, right
        integer :: i, j, pivot, temp

        if (left >= right) return

        pivot = A((left + right) / 2)

        i = left
        j = right

        do
            do while (A(i) < pivot)
                i = i + 1
            end do
            do while (A(j) > pivot)
                j = j - 1
            end do

            if (i >= j) exit

            temp = A(i)
            A(i) = A(j)
            A(j) = temp

            i = i + 1
            j = j - 1
        end do

        call quicksort(A, left, j)
        call quicksort(A, j + 1, right)
    end subroutine quicksort

end program QuickSortFromFile
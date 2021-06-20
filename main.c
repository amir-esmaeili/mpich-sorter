#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void display(int *array, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void sortArray(int numbers[], int size)
{

    for (size_t step = 1; step < size; step++)
    {
        int key = numbers[step];
        int j = step - 1;

        while (key < numbers[j] && j >= 0)
        {
            numbers[j + 1] = numbers[j];
            --j;
        }
        numbers[j + 1] = key;
    }
}

void mergeArrays(int *array1, int size1, int *array2, int size2, int *merged)
{

    int i = 0, j = 0, k = 0;
    while (i < size1 && j < size2)
    {
        if (array1[i] < array2[j])
        {
            merged[k++] = array1[i++];
        }
        else
        {
            merged[k++] = array2[j++];
        }
    }

    while (i < size1)
    {
        merged[k++] = array1[i++];
    }

    while (j < size2)
    {
        merged[k++] = array2[j++];
    }
}

int main()
{
    int size = 14;
    int unsortedArr[] = {1, 2, 5, 7, 3, 4, 10, 8, 5, 3, 2, 11, 12, 10};

    int size1 = size / 2;
    int size2 = size - size1;

    int arr1[size1];
    int arr2[size2];

    printf("size1: %d, size2: %d\n", size1, size2);

    for (size_t i = 0; i < size; i++)
    {
        if (i < size1)
        {
            arr1[i] = unsortedArr[i];
        }
        else
        {
            arr2[i - size1] = unsortedArr[i];
        }
    }

    sortArray(arr1, size1);
    sortArray(arr2, size2);

    int merged[size];
    mergeArrays(arr1, size1, arr2, size2, merged);

    display(merged, size);

    return 0;
}

int main(int argc, char **argv)
{

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    // Find out rank, size
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // We are assuming at least 3 processes for this task
    if (world_size < 3)
    {
        fprintf(stderr, "World size must be greater than 2 for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int size = 14;
    int input[] = {1, 2, 5, 7, 3, 4, 10, 8, 5, 3, 2, 11, 12, 10};
    int size1 = size / 2;
    int size2 = size - size1;

    if (world_rank == 0)
    {
        //provide data for world1

        int arr1[size1];
        int arr2[size2];

        for (size_t i = 0; i < size; i++)
        {
            if (i < size1)
            {
                arr1[i] = input[i];
            }
            else
            {
                arr2[i - size1] = input[i];
            }
        }

        //send the first half to world1
        MPI_Send(arr1, size1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        //send the second half to world2
        MPI_Send(arr2, size2, MPI_INT, 2, 0, MPI_COMM_WORLD);

        //recieve world1 result
        int world1_result[size1];
        MPI_Recv(&world1_result, size1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //recieve world2 result
        int world2_result[size2];
        MPI_Recv(&world2_result, size2, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int result[size];
        mergeArrays(world1_result, size1, world2_result, size2, result);

        display(result, size);
    }
    else if (world_rank == 1)
    {
        //recieve my_data from world0
        int my_data[size1];
        MPI_Recv(my_data, size1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //calculation
        sortArray(my_data, size1);
        display(my_data, size1);

        //sending data back to world0
        MPI_Send(&my_data, size1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        //recieve my_data from world0
        int my_data[size2];
        MPI_Recv(my_data, size2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //calculation
        sortArray(my_data, size2);
        display(my_data, size2);

        //sending data back to world0
        MPI_Send(&my_data, size2, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}
#include <stdio.h>
#include <stdlib.h>

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

    for (size_t i = 0; i < size; i++) {
        if (i < size1) {
            arr1[i] = unsortedArr[i];
        } else {
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
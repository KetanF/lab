#include <iostream>
#include <omp.h>
#include <vector>
#include <algorithm>

using namespace std;

void printArray(const vector<int> &arr)
{
    for (int i = 0; i < arr.size(); i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void sequentialBubbleSort(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void parallelBubbleSort(vector<int> &arr)
{
    int n = arr.size();
    // for (int i = 0; i < n - 1; i++) {
    //     #pragma omp parallel for
    //     for (int j = 0; j < n - i - 1; j++) {
    //         if (arr[j] > arr[j + 1]) {
    //             swap(arr[j], arr[j + 1]);
    //         }
    //     }
    // }

    for (int i = 0; i < n; i++)
    {
        int first = i % 2;
#pragma omp parallel for shared(arr, first)
        for (int j = first; j < n - 1; j += 2)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main()
{
    int n;
    double start_time, end_time;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> arr(n);
    for (int i = 0, j = n; i < n; i++, j--)
        arr[i] = j;

    vector<int> arr1 = arr; // Copy the array for sequential sort
    vector<int> arr2 = arr; // Copy the array for parallel sort

    start_time = omp_get_wtime();
    sequentialBubbleSort(arr1);
    end_time = omp_get_wtime();
    cout << "Sequential Bubble Sort took : " << end_time - start_time << " seconds.\n";

    start_time = omp_get_wtime();
    parallelBubbleSort(arr2);
    end_time = omp_get_wtime();
    cout << "Parallel Bubble Sort took : " << end_time - start_time << " seconds.\n";

    // Print the array before sorting if size is less than 20
    if (n <= 20)
    {
        cout << "\nArray before sorting: ";
        printArray(arr);
    }

    // Print the array after sorting if size is less than 20
    if (n <= 20)
    {
        cout << "Array after sequential sorting: ";
        printArray(arr1);
    }

    // Print the array after sorting if size is less than 20
    if (n <= 20)
    {
        cout << "Array after parallel sorting: ";
        printArray(arr2);
    }

    return 0;
}

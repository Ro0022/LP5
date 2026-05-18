// Parallel Merge Sort using OpenMP
// Compile:
// g++ -fopenmp merge_sort.cpp -o merge_sort
//
// Run:
// ./merge_sort

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

// Merge function
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int i = 0; i < n2; i++)
        R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];
}

// Sequential Merge Sort
void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    sequentialMergeSort(arr, left, mid);
    sequentialMergeSort(arr, mid + 1, right);

    merge(arr, left, mid, right);
}

// Parallel Merge Sort using OpenMP Tasks
void parallelMergeSort(vector<int>& arr, int left, int right, int depth) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    // Limit task creation depth
    if (depth <= 0) {
        sequentialMergeSort(arr, left, right);
        return;
    }

    #pragma omp task shared(arr)
    parallelMergeSort(arr, left, mid, depth - 1);

    #pragma omp task shared(arr)
    parallelMergeSort(arr, mid + 1, right, depth - 1);

    #pragma omp taskwait

    merge(arr, left, mid, right);
}

int main() {
    int n = 1000000; // Number of elements

    vector<int> arr1(n), arr2(n);

    srand(time(0));

    // Generate random numbers
    for (int i = 0; i < n; i++) {
        arr1[i] = rand() % 100000;
    }

    arr2 = arr1;

    // Sequential Merge Sort Timing
    double start = omp_get_wtime();

    sequentialMergeSort(arr1, 0, n - 1);

    double end = omp_get_wtime();

    double sequentialTime = end - start;

    // Parallel Merge Sort Timing
    start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        parallelMergeSort(arr2, 0, n - 1, 4);
    }

    end = omp_get_wtime();

    double parallelTime = end - start;

    // Output results
    cout << "Sequential Merge Sort Time: "
         << sequentialTime << " seconds" << endl;

    cout << "Parallel Merge Sort Time: "
         << parallelTime << " seconds" << endl;

    cout << "Speedup: "
         << sequentialTime / parallelTime << endl;

    return 0;
}
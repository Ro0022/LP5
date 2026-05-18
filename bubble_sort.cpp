#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <algorithm>

using namespace std;

// Function to print array
void printArray(const vector<int>& arr) {

    for (int x : arr)
        cout << x << " ";

    cout << endl;
}

// Sequential Bubble Sort
void sequentialBubbleSort(vector<int>& arr) {

    int n = arr.size();

    for (int i = 0; i < n - 1; i++) {

        bool swapped = false;

        for (int j = 0; j < n - i - 1; j++) {

            if (arr[j] > arr[j + 1]) {

                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // Stop if already sorted
        if (!swapped)
            break;
    }
}

// Parallel Bubble Sort using Odd-Even Transposition
void parallelBubbleSort(vector<int>& arr) {

    int n = arr.size();

    for (int phase = 0; phase < n; phase++) {

        // Even Phase
        if (phase % 2 == 0) {

#pragma omp parallel for default(none) shared(arr, n)
            for (int i = 0; i < n - 1; i += 2) {

                if (arr[i] > arr[i + 1]) {
                    swap(arr[i], arr[i + 1]);
                }
            }
        }

        // Odd Phase
        else {

#pragma omp parallel for default(none) shared(arr, n)
            for (int i = 1; i < n - 1; i += 2) {

                if (arr[i] > arr[i + 1]) {
                    swap(arr[i], arr[i + 1]);
                }
            }
        }
    }
}

int main() {

    int n;

    cout << "Enter size of array: ";
    cin >> n;

    // Input validation
    if (n <= 0) {

        cout << "Invalid array size!" << endl;
        return 0;
    }

    vector<int> arr(n);
    vector<int> arrSeq;
    vector<int> arrPar;

    srand(time(0));

    // Generate random numbers
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }

    // Print unsorted array
    cout << "\nUnsorted Array:\n";
    printArray(arr);

    // Copy arrays
    arrSeq = arr;
    arrPar = arr;

    // Sequential Bubble Sort Timing
    double startSeq = omp_get_wtime();

    sequentialBubbleSort(arrSeq);

    double endSeq = omp_get_wtime();

    // Parallel Bubble Sort Timing
    double startPar = omp_get_wtime();

    parallelBubbleSort(arrPar);

    double endPar = omp_get_wtime();

    // Verify correctness
    bool correct = (arrSeq == arrPar);

    // Performance Results
    cout << "\nSequential Bubble Sort Time: "
         << (endSeq - startSeq) * 1000000
         << " microseconds" << endl;

    cout << "Parallel Bubble Sort Time: "
         << (endPar - startPar) * 1000000
         << " microseconds" << endl;

    cout << "Speedup: "
         << (endSeq - startSeq) / (endPar - startPar)
         << endl;

    cout << "Sorting Correctness: ";

    if (correct)
        cout << "Verified" << endl;
    else
        cout << "Incorrect Result" << endl;

    // Print sorted array
    cout << "\nSorted Array:\n";
    printArray(arrPar);

    return 0;
}
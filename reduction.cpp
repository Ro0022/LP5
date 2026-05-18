#include <iostream>
#include <vector>
#include <climits>
#include <omp.h>

using namespace std;

int main() {
    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int minVal = INT_MAX;
    int maxVal = INT_MIN;
    long long sum = 0;

    double start, end;

    // Start time
    start = omp_get_wtime();

    // Parallel reduction
    #pragma omp parallel for reduction(min:minVal) reduction(max:maxVal) reduction(+:sum)
    for (int i = 0; i < n; i++) {
        if (arr[i] < minVal)
            minVal = arr[i];

        if (arr[i] > maxVal)
            maxVal = arr[i];

        sum += arr[i];
    }

    double average = (double)sum / n;

    // End time
    end = omp_get_wtime();

    cout << "\nResults using Parallel Reduction:\n";
    cout << "Minimum = " << minVal << endl;
    cout << "Maximum = " << maxVal << endl;
    cout << "Sum     = " << sum << endl;
    cout << "Average = " << average << endl;

    cout << "\nExecution Time: " << (end - start) << " seconds\n";

    return 0;
}
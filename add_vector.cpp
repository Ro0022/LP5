#include <iostream>
#include <cuda_runtime.h>

using namespace std;

#define N 5

// CUDA Kernel Function
__global__ void vectorAddition(int A[], int B[], int C[]) {

    // Thread ID
    int i = threadIdx.x;

    // Vector Addition
    C[i] = A[i] + B[i];
}

int main() {

    // Host Vectors (CPU)
    int A[N] = {1, 2, 3, 4, 5};
    int B[N] = {10, 20, 30, 40, 50};
    int C[N];

    // Device Vectors (GPU)
    int *d_A, *d_B, *d_C;

    // Allocate Memory on GPU
    cudaMalloc(&d_A, sizeof(A));
    cudaMalloc(&d_B, sizeof(B));
    cudaMalloc(&d_C, sizeof(C));

    // Copy Data from CPU to GPU
    cudaMemcpy(d_A, A, sizeof(A), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, sizeof(B), cudaMemcpyHostToDevice);

    // Launch Kernel
    vectorAddition<<<1, N>>>(d_A, d_B, d_C);

    // Copy Result from GPU to CPU
    cudaMemcpy(C, d_C, sizeof(C), cudaMemcpyDeviceToHost);

    // Print Result
    cout << "Vector Addition Result:\n";

    for (int i = 0; i < N; i++) {

        cout << A[i]
             << " + "
             << B[i]
             << " = "
             << C[i]
             << endl;
    }

    // Free GPU Memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}

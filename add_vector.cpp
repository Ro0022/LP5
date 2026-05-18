#include <iostream>
#include <cuda_runtime.h>

using namespace std;

__global__ void vectorAdd(int *A, int *B, int *C, int n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n)
    {
        C[i] = A[i] + B[i];
    }
}

int main()
{
    int n = 1000000;
    int size = n * sizeof(int);

    // Host arrays
    int *h_A = new int[n];
    int *h_B = new int[n];
    int *h_C = new int[n];

    // Initialize vectors
    for (int i = 0; i < n; i++)
    {
        h_A[i] = i;
        h_B[i] = i * 2;
    }

    // Device arrays
    int *d_A, *d_B, *d_C;

    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    // Copy data from host to device
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    // Kernel launch
    int threadsPerBlock = 256;
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;

    vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, n);

    // Copy result back
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    // Print first 10 results
    cout << "Vector Addition Result:" << endl;

    for (int i = 0; i < 10; i++)
    {
        cout << h_A[i] << " + " << h_B[i]
             << " = " << h_C[i] << endl;
    }

    // Free memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    delete[] h_A;
    delete[] h_B;
    delete[] h_C;

    return 0;
}
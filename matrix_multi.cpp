// Matrix Multiplication using CUDA
// Compile:
// nvcc matrix_mul.cu -o matrix_mul
//
// Run:
// ./matrix_mul

#include <iostream>
#include <cuda_runtime.h>

using namespace std;

#define N 3

__global__ void matrixMul(int *A, int *B, int *C)
{
    int row = threadIdx.y;
    int col = threadIdx.x;

    int sum = 0;

    for (int k = 0; k < N; k++)
    {
        sum += A[row * N + k] * B[k * N + col];
    }

    C[row * N + col] = sum;
}

int main()
{
    int size = N * N * sizeof(int);

    // Host matrices
    int h_A[N][N] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int h_B[N][N] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };

    int h_C[N][N];

    // Device matrices
    int *d_A, *d_B, *d_C;

    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    // Copy matrices to device
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    // Define block size
    dim3 threadsPerBlock(N, N);

    // Launch kernel
    matrixMul<<<1, threadsPerBlock>>>(d_A, d_B, d_C);

    // Copy result back
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    // Display result
    cout << "Matrix Multiplication Result:" << endl;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << h_C[i][j] << " ";
        }
        cout << endl;
    }

    // Free memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}
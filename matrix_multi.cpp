#include <iostream>
#include <cuda_runtime.h>

using namespace std;

#define N 2

__global__ void matrixMultiply(int A[N][N], int B[N][N], int C[N][N]) {

    int row = threadIdx.x;
    int col = threadIdx.y;

    C[row][col] = 0;

    for (int k = 0; k < N; k++) {

        C[row][col] += A[row][k] * B[k][col];
    }
}

int main() {

    int A[N][N] = {
        {1, 2},
        {3, 4}
    };

    int B[N][N] = {
        {5, 6},
        {7, 8}
    };

    int C[N][N];

    int (*d_A)[N];
    int (*d_B)[N];
    int (*d_C)[N];

    cudaMalloc(&d_A, sizeof(A));
    cudaMalloc(&d_B, sizeof(B));
    cudaMalloc(&d_C, sizeof(C));

    cudaMemcpy(d_A, A, sizeof(A), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, sizeof(B), cudaMemcpyHostToDevice);

    matrixMultiply<<<1, dim3(N, N)>>>(d_A, d_B, d_C);

    cudaMemcpy(C, d_C, sizeof(C), cudaMemcpyDeviceToHost);

    cout << "Result Matrix:\n";

    for (int i = 0; i < N; i++) {

        for (int j = 0; j < N; j++) {

            cout << C[i][j] << " ";
        }

        cout << endl;
    }

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}

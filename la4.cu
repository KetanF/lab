#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Sequential vector addition function
void vectorAddSequential(int* a, int* b, int* c, int size) {
    for (int i = 0; i < size; i++) {
        c[i] = a[i] + b[i];
    }
}

// CUDA kernel for vector addition
__global__ void vectorAddParallel(int* a, int* b, int* c, int size) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < size) {
        c[tid] = a[tid] + b[tid];
    }
}

int main() {
    int size;
    printf("Enter the size of the vectors: ");
    scanf("%d", &size);

    // Allocate memory for host vectors
    int* a = (int*)malloc(size * sizeof(int));
    int* b = (int*)malloc(size * sizeof(int));
    int* c = (int*)malloc(size * sizeof(int));

    // Initialize host vectors
    printf("Enter elements for vector a:\n");
    for (int i = 0; i < size; i++) {
        scanf("%d", &a[i]);
    }
    printf("Enter elements for vector b:\n");
    for (int i = 0; i < size; i++) {
        scanf("%d", &b[i]);
    }

    // Allocate memory on the device for device vectors
    int* dev_a, * dev_b, * dev_c;
    cudaMalloc((void**)&dev_a, size * sizeof(int));
    cudaMalloc((void**)&dev_b, size * sizeof(int));
    cudaMalloc((void**)&dev_c, size * sizeof(int));

    // Copy host vectors to device
    cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);

    // Parallel vector addition
    int blockSize = 256;
    int gridSize = (size + blockSize - 1) / blockSize;

    clock_t start_parallel = clock();
    vectorAddParallel<<<gridSize, blockSize>>>(dev_a, dev_b, dev_c, size);
    cudaDeviceSynchronize(); // Wait for the GPU to finish
    clock_t end_parallel = clock();
    double time_parallel = ((double)(end_parallel - start_parallel)) / CLOCKS_PER_SEC;

    // Copy result from device to host
    cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);

    // Sequential vector addition
    clock_t start_sequential = clock();
    vectorAddSequential(a, b, c, size);
    clock_t end_sequential = clock();
    double time_sequential = ((double)(end_sequential - start_sequential)) / CLOCKS_PER_SEC;

    // Print result
    printf("\nResult of Parallel Vector Addition:\n");
    for (int i = 0; i < size; i++) {
        printf("%d + %d = %d\n", a[i], b[i], c[i]);
    }

    printf("\nTime taken for Parallel Vector Addition: %lf seconds\n", time_parallel);
    printf("Time taken for Sequential Vector Addition: %lf seconds\n", time_sequential);

    // Free device memory
    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);

    // Free host memory
    free(a);
    free(b);
    free(c);

    return 0;
}


#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <string>

using namespace std;



__device__ int calc_square(int val)
{
    return val * val;
}

__global__ void square(int *numbers, int arraySize)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;
    int a;
    for (int i = idx; i < arraySize; i += stride)
    {
        a = calc_square(numbers[i]);
        numbers[i] = a;
    }
}

int main()
{
    const int arraySize = 100;
    int num[arraySize];

    for (int i = 0; i < 100; i++)
    {
        num[i] = i;
    }
    
    int* d_num;




    cudaMalloc((void**)&d_num,  arraySize * sizeof(int));
    cudaMemcpy(d_num, num, arraySize * sizeof(int), cudaMemcpyHostToDevice);
    square << <2, 5 >> > (d_num, arraySize);
    cudaDeviceSynchronize();


    cudaMemcpy(num, d_num, arraySize * sizeof(int), cudaMemcpyDeviceToHost); 
    printf("Processed array:\n");
    for (int i = 0; i < arraySize; i++)
    {
        printf("num[%d] = %d\n", i, num[i]);
    }

    // Free device memory
    cudaFree(d_num);

    return 0;
}


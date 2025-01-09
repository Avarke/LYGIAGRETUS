

/*

OpenMP du masyvai po 100 elem (1-100) (0.1-10). 20 threads. Sudaugint vienodus indeksus. Juos sudet i local sum. Naudot reduction kad sudet local sums i viena. 
B-pridet kad koks threadas koki didziausia indeksa duomenu naudojo  ir isprintint juos programos pabaigoj
reduction.


*/
#include <iostream>
#include <omp.h>
#include <thread>


using namespace std;

int main()
{
    int a[100];
    float b[100];
    float k = 0.1;
    float c[100];
    int arraySize = 100;
    int max_index[20];

    for (int i = 0; i < 100;  i++)
    {
        a[i] = i + 1;
    }
    for (int i = 0; i < 100; i++)
    {
        b[i] = k * (i + 1);
    }

    float totalSum = 0;
    omp_set_num_threads(20);
    #pragma omp parallel default(none) shared(a,b,c, arraySize, max_index) reduction(+:totalSum)
    {
        totalSum = 0;
        int threadId = omp_get_thread_num();
        int totalThreads = omp_get_num_threads();

        int itemsPer = arraySize / totalThreads;
        int remainder = arraySize % totalThreads;

        int start = threadId * itemsPer + (threadId < remainder ? threadId : remainder);
        int end = start + itemsPer + (threadId < remainder ? 1 : 0);

        
        for (int i = start; i < end; i++)
        {
            c[i] = a[i] * b[i];
            totalSum += c[i];;
        }

#pragma omp critical
        {
            max_index[threadId] = end - 1;
        }
    }

   /* for (int i = 0; i < arraySize; i++) {
        cout << c[i] << endl;
    }*/
    for (int i = 0; i < 20; i++) {
        cout << max_index[i] << endl;
    }


    //cout << totalSum << endl;

}



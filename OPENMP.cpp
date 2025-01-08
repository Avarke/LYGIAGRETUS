

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <chrono>
#include <functional>
#include <sstream>
#include <fstream>
#include <omp.h>
#include <array>
#include <cmath>
#include <iomanip>
#include <mutex>
#include <stdio.h>

using namespace std;



class Stack {

public:
    int elements[50];
    int currentIndex = -1;
    mutex stackMutex;


    void push(int i)
    {
        #pragma omp critical
        {
            if (currentIndex+1 < 50)
            {
                elements[++currentIndex] = i;
                cout << "Successfully added: " << i << endl;
            }
        }
    }
    int pop()
    {
        int a = -1;
        #pragma omp critical
        {
            if (currentIndex > -1)
            {
                a = elements[currentIndex];
                currentIndex--;
            }
        }
        return a;
    }



};

int main()
{
    
    Stack stack;
    int totalSum = 0;

    omp_set_num_threads(10);
    #pragma omp parallel default(none) shared(stack, cout) reduction(+:totalSum)
    {
        int threadNumber = omp_get_thread_num();
        for (auto i = 0; i < 5; i++)
        {
            totalSum += threadNumber;
            stack.push(threadNumber);                     
        }
        #pragma omp critical
        {
            cout << "Thread ended: " << threadNumber << endl;
        }    
    }
        
    // Print the final total sum
    cout << "Total Sum: " << totalSum << endl;

    // Print stack contents
    cout << "Stack contents (top to bottom): ";
    for (int i = stack.currentIndex; i >= 0; i--) {
        cout << stack.elements[i] << " ";
    }
    cout << endl;
}

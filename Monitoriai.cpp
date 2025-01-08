

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <condition_variable>


using namespace std;

atomic <int> activeWorkers;


class Queue {

public:
    int numbers[10];
    int current_index = -1;
    mutex queue_mutex;
    condition_variable printable;

    condition_variable removable;
    condition_variable addable;


    int returnMax()
    {
        if (current_index == -1)
            return -1;
        unique_lock<mutex> lock(queue_mutex);
        printable.wait(lock, [&] { return current_index > -1; });
        int a = numbers[0];
        for (int i = 0; i <= current_index; i++)
        {
            if (numbers[i] > a)
                a = numbers[i];
        }
        return a;
    }

    void push(int a)
    {
        unique_lock<mutex> lock(queue_mutex);
        addable.wait(lock, [&] {return current_index+1 < 10; });
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        numbers[++current_index] = a;
        removable.notify_all();
       
    }

    int pop()
    {
        unique_lock<mutex> lock(queue_mutex);
        removable.wait(lock, [&] {return current_index > -1; });
        
        int a = numbers[current_index];
        current_index--;

        addable.notify_all();
        return a;
    }
};


void Worker(Queue& queue, int a)
{
    activeWorkers++;
    for (int i = 0; i < 5; i++)
    {
        queue.push(a);
    }
    activeWorkers--;
    //cout << "Worker finished" << endl;
}


void Printer(Queue& queue)
{
    while (true)
    {
        int a = queue.returnMax();
        cout << "Max: " << a << endl;
        if (activeWorkers == 0)
            break;
    }
}


void Remover(Queue& queue)
{
    for (int i = 0; i < 40; i++)
    {
        int a = queue.pop();
        cout << "Popped: " << a << endl;
    }
    //cout << "Remover finished" << endl;
}


int main()
{
    Queue queue;
    vector<thread> Threads;
    //Threads.emplace_back(Printer, ref(queue));
    Threads.emplace_back(Remover, ref(queue));
    for (int i = 0; i < 10; i++)
    {
        Threads.emplace_back(Worker, ref(queue), i);
    }

    for ( auto& var : Threads)
    {
        var.join();
    }

    cout << "Finished" << endl;
}


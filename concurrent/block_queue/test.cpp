#include <iostream>
#include <thread>
#include "block_queue.h"
using namespace std;

#define push_times 50000
#define pop_times 49999
#define push_num 10
#define pop_num 10

BlockQueue<int> q(10000);

void threadPush()
{
    int n = 1;
    for (int i = 0; i < push_times; ++i)
    {
        q.push(n);
    }
}

void threadPop()
{
    int n;
    for (int i = 0; i < pop_times; ++i)
    {
        q.pop(n);
    }
}

int main()
{
    thread pushs[push_num];
    thread pops[pop_num];
    for (int i = 0; i < push_num; ++i)
    {
        pushs[i] = thread(threadPush);
    }
    for (int i = 0; i < pop_num; ++i)
    {
        pops[i] = thread(threadPop);
    }

    for (int i = 0; i < push_num; ++i)
    {
        pushs[i].join();
    }
    for (int i = 0; i < push_num; ++i)
    {
        pops[i].join();
    }
    cout << q.size() << endl;
}
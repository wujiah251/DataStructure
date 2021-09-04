#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include "block_queue.h"
using namespace std;

BlockQueue<int> q(10000);

class Test
{
private:
    int num_of_writers;
    int write_iters;
    double push_persent;
    unsigned int seed;
    BlockQueue<int> *q;
    static void write(BlockQueue<int> *q, int write_iters, double push_persent, unsigned int seed)
    {
        for (int i = 0; i < write_iters; ++i)
        {
            int n;
            double num = rand_r(&seed) / (double)RAND_MAX;
            if (num < push_persent)
            {
                q->push(1);
            }
            else
            {
                q->pop(n);
            }
        }
    }

public:
    Test(BlockQueue<int> *q, int num_of_writers, int write_iters, unsigned int seed, double push_persent)
        : q(q), num_of_writers(num_of_writers), write_iters(write_iters), seed(seed), push_persent(push_persent) {}
    double Run()
    {
        clock_t start = clock();
        vector<thread> writers(num_of_writers);
        for (int i = 0; i < num_of_writers; ++i)
        {
            writers[i] = thread(write, q, write_iters, push_persent, seed);
        }
        for (int i = 0; i < num_of_writers; ++i)
        {
            writers[i].join();
        }
        clock_t end = clock();
        double res = ((double)end - start) / CLOCKS_PER_SEC;
        return (num_of_writers * write_iters) / res;
    }
};

int main()
{
    unsigned int seed = time(NULL);
    Test t(&q, 10, 100, seed, 0.6);
    cout << t.Run() << endl;
    return 0;
}
#include <iostream>
#include <functional>
#include <thread>
#include <vector>
using namespace std;

class Test
{
private:
    vector<function<void(void *)>> functions;

public:
    Test(vector<function<void(void *)>> functions) : functions(functions) {}
    double Run(vector<void *> &input)
    {
        clock_t start = clock();
        if (input.size() != functions.size())
            return -1;
        vector<thread> threads(input.size());
        for (int i = 0; i < functions.size(); ++i)
        {
            threads[i] = thread(functions, input[i]);
        }
        for (int i = 0; i < functions.size(); ++i)
        {
            threads[i].join();
        }
        clock_t end = clock();
        return ((double)end - start) / CLOCKS_PER_SEC;
    }
};

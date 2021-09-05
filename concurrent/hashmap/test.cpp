#include "hashmap.h"
#include "shared_hashmap.h"
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <ctime>
#include <unordered_map>
using namespace std;

struct zone_hash
{
    size_t operator()(int key)
    {
        return static_cast<size_t>(key);
    }
};

class Test
{
private:
    int num_of_writers;
    int num_of_readers;
    int write_iters;
    int read_iters;
    int left;
    int right;
    unsigned int seed;
    Map<int, int> *map;

public:
    Test(int W, int R, int w, int r, unsigned int seed, Map<int, int> *map, int left, int right)
        : num_of_writers(W), num_of_readers(R), write_iters(w), read_iters(r), seed(seed), map(map), left(left), right(right) {}
    double Run()
    {
        clock_t start = clock();
        vector<thread> writers(num_of_writers), readers(num_of_readers);
        for (int i = 0; i < num_of_writers; ++i)
        {
            writers[i] = thread(write, write_iters, seed, left, right, map);
        }
        for (int i = 0; i < num_of_readers; ++i)
        {
            readers[i] = thread(read, read_iters, seed + 1, left, right, map);
        }
        for (int i = 0; i < num_of_writers; ++i)
        {
            writers[i].join();
        }
        for (int i = 0; i < num_of_readers; ++i)
        {
            readers[i].join();
        }
        clock_t end = clock();
        double res = ((double)end - start) / CLOCKS_PER_SEC;
        return (num_of_writers * write_iters + num_of_readers * read_iters) / res;
    }
    static void write(int write_iters, unsigned int seed, int left, int right, Map<int, int> *map)
    {
        int key, val;
        for (int i = 0; i < write_iters; ++i)
        {
            key = rand_r(&seed) % (right - left + 1) + left;
            val = rand_r(&seed) % (right - left + 1) + left;
            map->put(key, val);
        }
    }
    static void read(int read_iters, unsigned int seed, int left, int right, Map<int, int> *map)
    {
        int key, val;
        for (int i = 0; i < read_iters; ++i)
        {
            key = rand_r(&seed) % (right - left + 1) + left;
            map->get(key, val);
        }
    }
};

int main()
{
    HashMap<int, int> map1;
    unsigned int seed = 10;
    Test t1(10, 10, 100000, 100000, seed, &map1, 0, 500);
    double res1 = t1.Run();
    cout << res1 << endl;
    SharedHashMap<int, int, zone_hash> map2(16, 0.75);
    Test t2(10, 10, 100000, 100000, 1, &map2, 0, 500);
    double res2 = t2.Run();
    cout << res2 << endl;
    unordered_map<int, int> map;
    return 0;
}
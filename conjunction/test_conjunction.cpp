#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <benchmark/benchmark.h>
#include "conjunction_base.h"
#include "conjunction_skip.h"
#include "conjunction_traverse.h"
#include "conjunction_zipper.h"

static int low_bound = 0;
static int up_bound = 10000;
#define N 10
static int len = 1000;

void init(std::vector<std::vector<int>> &multi_set, int n = N)
{
    srand(time(NULL));
    multi_set.resize(n);
    for (int i = 0; i < n; ++i)
    {
        multi_set[i].resize(len);
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < len; ++j)
        {
            multi_set[i][j] = rand() % (up_bound - low_bound + 1) + low_bound;
        }
        std::sort(multi_set[i].begin(), multi_set[i].end());
    }
}

void test(std::vector<std::vector<int>> &multi_set, std::vector<int> &res, Conjunction<int> *impl)
{
    impl->intersection(multi_set, res);
}

static void traverse_test(benchmark::State &state)
{
    Conjunction<int> *impl = new ConjunctionTraverse<int>;
    std::vector<std::vector<int>> multi_set;
    for (auto _ : state)
    {
        state.PauseTiming();
        init(multi_set);
        std::vector<int> res;
        state.ResumeTiming();
        test(multi_set, res, impl);
    }
    delete impl;
}

static void zipper_test(benchmark::State &state)
{
    Conjunction<int> *impl = new ConjunctionZipper<int>;
    std::vector<std::vector<int>> multi_set;
    for (auto _ : state)
    {
        state.PauseTiming();
        init(multi_set, 2);
        std::vector<int> res;
        state.ResumeTiming();
        test(multi_set, res, impl);
    }
    delete impl;
}

static void skip_test(benchmark::State &state)
{
    Conjunction<int> *impl = new ConjunctionSkip<int>;
    std::vector<std::vector<int>> multi_set;
    for (auto _ : state)
    {
        state.PauseTiming();
        init(multi_set);
        std::vector<int> res;
        state.ResumeTiming();
        test(multi_set, res, impl);
    }
    delete impl;
}

BENCHMARK(traverse_test);
BENCHMARK(zipper_test);
BENCHMARK(zipper_test);
BENCHMARK_MAIN();
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <assert.h>
#include "conjunction_base.h"
#include "conjunction_skip.h"
#include "conjunction_traverse.h"
#include "conjunction_zipper.h"
using namespace std;

static int low_bound = 0;
static int up_bound = 10000;
static int N = 10;
static int len = 1000;

void init(vector<vector<int>> &multi_set)
{
    srand(time(NULL));
    multi_set.resize(N);
    for (int i = 0; i < N; ++i)
    {
        multi_set[i].resize(len);
    }
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < len; ++j)
        {
            multi_set[i][j] = rand() % (up_bound - low_bound + 1) + low_bound;
        }
        sort(multi_set[i].begin(), multi_set[i].end());
    }
}

void test(vector<vector<int>> &multi_set, vector<int> &res, Conjunction<int> *impl)
{
    impl->intersection(multi_set, res);
}

int main()
{
    vector<vector<int>> sets{{1, 3, 5, 7, 9}, {1, 2, 3, 4, 7, 8, 10}, {1, 2, 3, 4, 5, 6, 7, 8, 9}};
    vector<int> res;
    vector<int> desired_res{1, 3, 7};
    ConjunctionSkip<int> impl1;
    ConjunctionTraverse<int> impl2;
    ConjunctionZipper<int> impl3;

    auto temp = sets;
    impl1.intersection(temp, res);
    assert(res == desired_res);
    temp = sets;
    impl2.intersection(temp, res);
    assert(res == desired_res);
    temp = sets;
    impl3.intersection(temp, res);
    assert(res == desired_res);

    init(sets);
    vector<int> res1, res2, res3;
    auto set1 = sets;
    auto set2 = sets;
    auto set3 = sets;
    impl1.intersection(set1, res1);
    impl2.intersection(set2, res2);
    impl3.intersection(set3, res3);
    assert(res1 == res2);
    assert(res2 == res3);
    return 0;
}
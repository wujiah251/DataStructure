#include "lfu.h"
#include <iostream>
using namespace std;

int main()
{
    LFUCache<int, int> cache(4);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3);
    cache.put(4, 4);
    int v;
    cache.get(1, v);
    cache.get(2, v);
    cache.get(3, v);
    cache.put(5, 5);
    if (cache.get(1, v))
    {
        cout << v << endl;
    }
    if (cache.get(2, v))
    {
        cout << v << endl;
    }
    if (cache.get(3, v))
    {
        cout << v << endl;
    }
    if (cache.get(4, v))
    {
        cout << v << endl;
    }
    if (cache.get(5, v))
    {
        cout << v << endl;
    }
    return 0;
}
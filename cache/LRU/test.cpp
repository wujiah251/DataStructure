#include <iostream>
#include "lru1.h"

int main()
{
    LRUCache<int, int> cache(3);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3);
    cache.dump();
    cache.put(4, 4);
    cache.dump();
    return 0;
}
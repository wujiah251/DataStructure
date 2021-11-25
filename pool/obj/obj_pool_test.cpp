#include <iostream>
#include "obj_pool.h"

int main()
{
    struct node
    {
        int a;
        int b;
        node() {}
        node(int a, int b) : a(a), b(b) {}
    };
    ObjPool<node> pool(5);
    std::cout << "Hello,World!" << std::endl;
    return 0;
}
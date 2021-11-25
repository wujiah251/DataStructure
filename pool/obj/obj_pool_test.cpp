#include <iostream>
#include <assert.h>
#include "obj_pool.h"

int main()
{
    struct n
    {
        int a;
        int b;
        n() {}
        n(int a, int b) : a(a), b(b) {}
    };
    std::cout << "------------Start Test------------" << std::endl;
    ObjPool<n> pool(5);
    auto node1 = pool.alloc(1, 2);
    auto node2 = pool.alloc(2, 3);
    auto node3 = pool.alloc(3, 4);
    auto node4 = pool.alloc(4, 5);
    auto node5 = pool.alloc(5, 6);
    assert((long)node2 - (long)node1 == 8);
    assert((long)node3 - (long)node2 == 8);
    assert((long)node4 - (long)node3 == 8);
    assert((long)node5 - (long)node4 == 8);
    assert(node1->a == 1);
    assert(node1->b == 2);
    assert(node2->a == 2);
    assert(node2->b == 3);
    assert(node3->a == 3);
    assert(node3->b == 4);
    assert(node4->a == 4);
    assert(node4->b == 5);
    assert(node5->a == 5);
    assert(node5->b == 6);
    pool.release(node1);
    pool.release(node2);
    pool.release(node3);
    pool.release(node4);
    pool.release(node5);
    std::cout << "-------------End Test-------------" << std::endl;
    return 0;
}
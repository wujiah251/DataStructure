#ifndef FREE_QUEUE_H_
#define FREE_QUEUE_H_

#include <cstddef>
#include <emmintrin.h>

struct AtomicNode
{
    volatile __intptr_t next;
    void *data;
};

template <class T>
class FreeQueue
{
private:
    volatile __intptr_t tail_;
    volatile __intptr_t head_;

public:
    FreeQueue();
    ~FreeQueue();
    void Enqueue(AtomicNode *node);
    AtomicNode *Dequeue();
};

#endif

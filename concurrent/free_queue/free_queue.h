#ifndef FREE_QUEUE_H_
#define FREE_QUEUE_H_
#include <cstddef>

template <class T>
class FreeQueue
{
private:
    size_t size_;
    int front_;
    int rear_;
    T *array_;

public:
    void push(const T &item);
    void pop(T &item);
    size_t size();
    bool empty();
};

#endif
#include <stdio.h>
#include <thread>
#include <unistd.h>
#include <iostream>
#include <mutex>
using namespace std;

#define RING_QUEUE_SIZE 10
//std::mutex mtx;
template <class T>
class RingBuffer
{
public:
    RingBuffer(int size) : m_size(size), m_head(0), m_tail(0)
    {
        m_buf = new T[size];
    }

    ~RingBuffer()
    {
        delete[] m_buf;
        m_buf = NULL;
    }

    inline bool isEmpty() const
    {
        return m_head == m_tail;
    }

    inline bool isFull() const
    {
        return m_tail == (m_head + 1) % m_size; //取模是为了考虑队列尾的特殊情况
    }

    /*使用互斥锁实现并发情况共用队列*/
    /*使用原子操作实现并发情况无锁队列*/
    bool cas_push(const T &value);
    bool cas_push(const T *value);
    bool cas_pop(T &value);

    inline unsigned int head() const
    {
        return m_head;
    }
    inline unsigned int tail() const
    {
        return m_tail;
    }
    inline unsigned int size() const
    {
        return m_size;
    }

private:
    int m_size; // 队列大小
    int m_head; // 队列头部索引
    int m_tail; // 队列尾部索引
    T *m_buf;   // 队列数据缓冲区
};
template <class T>
bool RingBuffer<T>::cas_push(const T &value)
{
    if (isFull())
    {
        return false;
    }
    int oldValue, newValue;
    do
    {
        oldValue = m_head;
        newValue = (oldValue + 1) % m_size;
    } while (__sync_bool_compare_and_swap(&m_head, oldValue, newValue) != true);
    m_buf[oldValue] = value;
    return true;
}

template <class T>
bool RingBuffer<T>::cas_push(const T *value)
{
    if (isFull())
    {
        return false;
    }
    int oldValue, newValue;
    do
    {
        oldValue = m_head;
        newValue = (oldValue + 1) % m_size;
    } while (__sync_bool_compare_and_swap(&m_head, oldValue, newValue) != true);
    m_buf[oldValue] = *value;
    return true;
}

template <class T>
bool RingBuffer<T>::cas_pop(T &value)
{
    if (isEmpty())
    {
        return false;
    }
    int oldValue, newValue;
    do
    {
        oldValue = m_tail;
        newValue = (oldValue + 1) % m_size;
    } while (__sync_bool_compare_and_swap(&m_tail, oldValue, newValue) != true);
    value = m_buf[oldValue];
    return true;
}

typedef struct Node
{ //任务节点
    int cmd;
    void *value;
} taskNode;

void produce(RingBuffer<taskNode> *rqueue)
{
    int i = 0;
    for (i = 0; i < RING_QUEUE_SIZE; i++)
    {
        taskNode node;
        node.cmd = i;
        rqueue->cas_push(node);
    }
}

void consume(RingBuffer<taskNode> *rqueue)
{
    while (!rqueue->isEmpty())
    {
        taskNode node;
        rqueue->cas_pop(node);
    }
}

int main()
{
    int i = 0;
    RingBuffer<taskNode> *rqueue = new RingBuffer<taskNode>(RING_QUEUE_SIZE);
    std::thread producer[20];
    std::thread consumer[20];
    for (i = 0; i < 20; i++)
    {
        producer[i] = std::thread(produce, rqueue);
        consumer[i] = std::thread(consume, rqueue);
    }
    for (auto &thread : producer)
        thread.join();
    for (auto &thread : consumer)
        thread.join();
    delete rqueue;
    return 0;
}
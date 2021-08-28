#ifndef BLOCK_QUEUE_H_
#define BLOCK_QUEUE_H_

#include <mutex>
#include <condition_variable>

template <class T>
class BlockQueue
{
private:
    std::mutex mutex_;
    std::condition_variable cond_no_empty_;
    std::condition_variable cond_no_full_;
    T *array_;
    int size_;
    int front_;
    int rear_;
    int max_size_;

public:
    BlockQueue(int max_size = 10000) : max_size_(max_size), size_(0), front_(0), rear_(-1)
    {
        array_ = new T[max_size_];
    }
    ~BlockQueue()
    {
        std::lock_guard<std::mutex> guard(mutex_);
        if (array_ != nullptr)
            delete[] array_;
    }
    void push(const T &item);
    void pop(T &item);
    bool front(T &item);
    bool back(T &item);
    void clear();
    int size()
    {
        std::lock_guard<std::mutex> guard(mutex_);
        return size_;
    }
    bool empty()
    {
        std::lock_guard<std::mutex> guard(mutex_);
        return size_ = 0;
    }
};

template <class T>
void BlockQueue<T>::push(const T &item)
{
    std::unique_lock<std::mutex> lck(mutex_);
    while (size_ >= max_size_)
    {
        // 等待非满
        cond_no_full_.wait(lck);
    }
    rear_ = (rear_ + 1) % max_size_;
    array_[rear_] = item;
    ++size_;
    cond_no_empty_.notify_one();
}

template <class T>
void BlockQueue<T>::pop(T &item)
{
    std::unique_lock<std::mutex> lck(mutex_);
    while (size_ <= 0)
    {
        cond_no_empty_.wait(lck);
    }
    item = array_[front_];
    front_ = (front_ + 1) % max_size_;
    --size_;
    cond_no_full_.notify_one();
    lck.unlock();
}

template <class T>
void BlockQueue<T>::clear()
{
    std::lock_guard<std::mutex> guard(mutex_);
    front_ = 0;
    rear_ = -1;
    size_ = 0;
}

template <class T>
bool BlockQueue<T>::front(T &item)
{
    std::lock_guard<std::mutex> guard(mutex_);
    if (size_ <= 0)
        return false;
    item = array_[front_];
    return true;
}

template <class T>
bool BlockQueue<T>::back(T &item)
{
    std::lock_guard<std::mutex> guard(mutex_);
    if (size_ <= 0)
        return false;
    item = array_[rear_];
    return true;
}

#endif
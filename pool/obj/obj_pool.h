#ifndef OBJ_POOL_H_
#define OBJ_POOL_H_

#include <cstddef>
#include <utility>

template <class T>
class ObjPool
{
public:
    ObjPool(size_t N)
        : size_(N), head_(0), free_(N), data_(new node[N])
    {
        for (int i = 0; i < N - 1; ++i)
        {
            data_[i].next_ = i + 1;
        }
        // next point to self means tail
        data_[N - 1].next_ = N - 1;
    }
    virtual ~ObjPool()
    {
        delete data_;
    }
    T *alloc()
    {
        if (free_ <= 0)
        {
            return nullptr;
        }
        --free_;
        T *res = data_[head_];
        head_ = data_[head_].next;
        return new (res) T;
    }
    template <typename... Params>
    T *alloc(Params &&...params)
    {
        if (free_ <= 0)
        {
            return nullptr;
        }
        --free_;
        T *res = data_[head_];
        head_ = data_[head_].next;
        return new (res) T(std::forward<Params>(params)...);
    }
    void release(T *obj)
    {
        if ((node *)obj - data_ >= size_)
        {
            return;
        }
        node *cur = static_cast<node *>(obj);
        cur->next = head_;
        head_ = cur - data_;
        ++free_;
        return;
    }

private:
    union node
    {
        T obj_;
        size_t next_;
        node() {}
    };
    size_t size_;
    size_t free_;
    size_t head_;
    node *data_;
};

#endif
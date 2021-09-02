#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <functional>
#include <vector>
#include <mutex>
#include <limits.h>
#include "map.h"
using std::lock_guard;
using std::mutex;
using std::vector;

template <typename Key, typename Value,
          typename Hash = std::hash<Key>>
class HashMap : public Map<Key, Value>
{
private:
    struct node
    {
        Key key;
        Value val;
        size_t hash_code;
        node *next;
        node(Key key, Value val, size_t hash_code, node *next = nullptr)
            : key(key), val(val), hash_code(hash_code), next(next) {}
    };
    mutex mutex_;
    size_t size_;
    Hash hash_;
    double load_factor_;
    vector<node *> buckets_;
    size_t index(node *cur) { return cur->hash_code & (buckets_.size() - 1); }
    void resize();
    void clear();

public:
    HashMap(double load_factor = 0.75);
    ~HashMap();
    void put(const Key &key, const Value &val);
    bool get(const Key &key, Value &res);
    void remove(const Key &key);
    size_t size();
};

template <typename Key, typename Value, typename Hash>
HashMap<Key, Value, Hash>::HashMap(double load_facotr)
    : load_factor_(load_facotr), size_(0), buckets_(4, nullptr)
{
}
template <typename Key, typename Value, typename Hash>
HashMap<Key, Value, Hash>::~HashMap()
{
    clear();
}

template <typename Key, typename Value, typename Hash>
void HashMap<Key, Value, Hash>::resize()
{
    if (static_cast<double>(size_) / buckets_.size() < load_factor_)
        return;
    size_t new_size = 4;
    while (static_cast<double>(size_) / new_size < load_factor_ && new_size * 2 < INT_MAX)
    {
        new_size *= 2;
    }
    if (new_size <= buckets_.size())
        return;
    vector<node *> new_buckets(new_size, nullptr);
    for (int i = 0; i < buckets_.size(); ++i)
    {
        node *cur = buckets_[i], *temp;
        while (cur)
        {
            size_t index = cur->hash_code & (new_size - 1);
            temp = cur->next;
            cur->next = new_buckets[index];
            new_buckets[index] = cur;
            cur = temp;
        }
    }
}

template <typename Key, typename Value, typename Hash>
void HashMap<Key, Value, Hash>::clear()
{
    lock_guard<mutex> guard(mutex_);
    node *temp;
    for (node *cur : buckets_)
    {
        while (cur)
        {
            temp = cur;
            cur = cur->next;
            delete temp;
        }
    }
}

template <typename Key, typename Value, typename Hash>
void HashMap<Key, Value, Hash>::put(const Key &key, const Value &val)
{
    lock_guard<mutex> guard(mutex_);
    size_t hash_code = hash_(key);
    size_t index = hash_code & (buckets_.size() - 1);
    node *cur = buckets_[index];
    while (cur)
    {
        if (cur->key == key)
        {
            cur->val = val;
            return;
        }
        cur = cur->next;
    }
    ++size_;
    cur = new node(key, val, hash_(key), buckets_[index]);
    buckets_[index] = cur;
    resize();
}

template <typename Key, typename Value, typename Hash>
bool HashMap<Key, Value, Hash>::get(const Key &key, Value &res)
{
    lock_guard<mutex> guard(mutex_);
    size_t hash_code = hash_(key);
    size_t index = hash_code & (buckets_.size() - 1);
    node *cur = buckets_[index];
    while (cur)
    {
        if (cur->key == key)
        {
            res = cur->val;
            return true;
        }
        cur = cur->next;
    }
    return false;
}

template <typename Key, typename Value, typename Hash>
void HashMap<Key, Value, Hash>::remove(const Key &key)
{
    lock_guard<mutex> guard(mutex_);
    size_t hash_code = hash_(key);
    size_t index = hash_code & (buckets_.size() - 1);
    node *pre = buckets_[index];
    if (!pre)
        return;
    if (pre->key == key)
    {
        buckets_[index] = pre->next;
        delete pre;
    }
    while (pre->next)
    {
        if (pre->next->key == key)
        {
            node *temp = pre->next;
            pre->next = temp->next;
            delete temp;
            break;
        }
        pre = pre->next;
    }
}

template <typename Key, typename Value, typename Hash>
size_t HashMap<Key, Value, Hash>::size()
{
    lock_guard<mutex> guard(mutex_);
    return size_;
}

#endif
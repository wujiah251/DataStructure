#ifndef SKIPLIST_H_
#define SKIPLIST_H_

#include <cstddef>
#include <vector>
using namespace std;

template <class Key, class Value, class Comparator>
class SkipList
{
private:
    struct node
    {
        Key k;
        Value v;
        size_t level;
        node *next;
        node(Key k, Value v, size_t l) : k(k), v(v), level(l) { next = new node[level]; }
        ~node() { delete[] next; }
    };
    const size_t max_level_;
    node head_;
    Comparator comp_;
    size_t size_;

public:
    SkipList(size_t max_level);
    ~SkipList();
    bool get(const Key &k, Value &res);
    void put(const Key &k, const Value &v);
    void remove(const Key &k);
    size_t size();
};

template <class Key, class Value, class Comparator>
SkipList<Key, Value, Comparator>::SkipList(size_t max_level) : size_(0), max_level_(max_level), node(Key(), Value(), max_level_ + 1)
{
    for (int i = 0; i <= head_.level; ++i)
    {
        head_.next[i] = nullptr;
    }
}

template <class Key, class Value, class Comparator>
void SkipList<Key, Value, Comparator>::put(const Key &k, const Value &v)
{
    node *cur = &head_;
    size_t level = max_level_;
    vector<node *> updates(max_level_ + 1);
    while (true)
    {
        node *temp;
        temp = (cur->next)[level];
        if (temp && comp_(temp->k, k))
        {
                }
    }
}
#endif
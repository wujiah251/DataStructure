#ifndef SKIPLIST_H_
#define SKIPLIST_H_

#include <cstddef>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <time.h>

#define MAX_LEVEL 32

template <class Key, class Value>
class SkipList
{
private:
    struct node
    {
        Key k;
        Value v;
        size_t level;
        node **next; // 保存指向下一个
        node(Key k, Value v, size_t l) : k(k), v(v), level(l) { next = new node *[level]; }
        ~node() { delete[] next; }
    };
    const size_t max_level_;
    node head_;
    size_t size_;

public:
    SkipList(size_t max_level = MAX_LEVEL);
    ~SkipList();
    bool get(const Key &k, Value &res);
    void put(const Key &k, const Value &v);
    void remove(const Key &k);
    size_t size() { return size_; }
    void print();

private:
    node *create_node(const Key &k, const Value &v)
    {
        size_t level = 1;
        while (level < max_level_)
        {
            if ((double)rand() / RAND_MAX < 0.5)
            {
                ++level;
            }
            else
            {
                break;
            }
        }
        return new node(k, v, level);
    }
};

template <class Key, class Value>
SkipList<Key, Value>::SkipList(size_t max_level)
    : size_(0), max_level_(max_level),
      head_(Key(), Value(), max_level_)
{
    for (int i = 0; i < head_.level; ++i)
    {
        head_.next[i] = nullptr;
    }
    srand(time(NULL));
}

template <class Key, class Value>
SkipList<Key, Value>::~SkipList()
{
    node *cur, *temp;
    cur = head_.next[0];
    while (cur)
    {
        temp = cur->next[0];
        delete cur;
        cur = temp;
    }
}

template <class Key, class Value>
bool SkipList<Key, Value>::get(const Key &k, Value &res)
{
    node *cur = &head_, *temp;
    int level = max_level_ - 1;
    for (; level >= 0; --level)
    {
        temp = cur->next[level];
        while (temp && (temp->k < k))
        {
            cur = temp;
            temp = cur->next[level];
        }
        if (temp && temp->k == k)
        {
            res = temp->v;
            return true;
        }
    }
    return false;
}

template <class Key, class Value>
void SkipList<Key, Value>::put(const Key &k, const Value &v)
{
    node *cur = &head_, *temp;
    int level = max_level_ - 1;
    std::vector<node *> updates(max_level_);
    for (; level >= 0; --level)
    {
        temp = cur->next[level];
        while (temp && (temp->k < k))
        {
            cur = temp;
            temp = cur->next[level];
        }
        updates[level] = cur;
    }
    if (cur->next[0] && cur->next[0]->k == k)
    {
        cur->next[0]->v = v;
        return;
    }
    node *new_node = create_node(k, v);
    for (int i = 0; i < new_node->level; ++i)
    {
        new_node->next[i] = updates[i]->next[i];
        updates[i]->next[i] = new_node;
    }
    ++size_;
    return;
}

template <class Key, class Value>
void SkipList<Key, Value>::remove(const Key &k)
{
    node *cur = &head_, *temp;
    size_t level = max_level_ - 1;
    std::vector<node *> updates(max_level_);
    for (; level >= 0; --level)
    {
        temp = cur->next[level];
        while (temp && temp->k < k)
        {
            // if next > cur,move to next
            // else level--
            cur = temp;
            temp = cur->next[level];
        }
        updates[level] = cur;
    }
    if (cur->next[0] && cur->next[0]->k == k)
    {
        node *need_delete = cur->next[0];
        for (int i = 0; i < need_delete->level; ++i)
        {
            updates[i]->next[i] = need_delete->next[i];
        }
        delete need_delete;
        size_--;
        return;
    }
}

template <class Key, class Value>
void SkipList<Key, Value>::print()
{
    std::cout << "print skiplist:" << std::endl;
    node *cur = head_.next[0];
    while (cur)
    {
        for (int i = 0; i < cur->level; ++i)
        {
            std::cout << "(" << cur->k << "," << cur->v << ")"
                      << "\t";
        }
        std::cout << std::endl;
        cur = cur->next[0];
    }
    std::cout << "end print" << std::endl;
}

#endif
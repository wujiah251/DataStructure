#ifndef LFU_H_
#define LFU_H_

#include <unordered_map>
#include <limits.h>
using std::unordered_map;

template <class Key, class Value>
class LFUCache
{
private:
    struct head_node;
    struct node
    {
        node *prev;
        node *next;
        head_node *head;
        Key key;
        Value val;
        node(Key key, Value val, node *p = nullptr, node *n = nullptr, head_node *h = nullptr) : prev(p), next(n), head(h), key(key), val(val) {}
    };
    struct head_node
    {
        int count;
        node *cur;
        head_node *prev;
        head_node *next;
        head_node(int count, node *cur = nullptr, head_node *prev = nullptr, head_node *next = nullptr) : count(count), cur(cur), prev(prev), next(next) {}
    };
    head_node *head;
    head_node *tail;
    unordered_map<Key, node *> map;
    int capacity;
    int size;
#define MIN_SIZE 1

public:
    LFUCache(int cap) : size(0)
    {
        head = new head_node(0);
        tail = new head_node(INT_MAX);
        head->next = tail;
        tail->prev = head;
        capacity = cap < MIN_SIZE ? MIN_SIZE : cap;
    }
    ~LFUCache()
    {
        head_node *cur_head = head->next;
        while (cur_head != tail)
        {
            head_node *temp = cur_head->next;
            erase(cur_head);
            cur_head = temp;
        }
        delete head;
        delete tail;
    }
    bool get(Key k, Value &res);
    void put(Key k, Value v);
    int length() { return size; }

private:
    void erase(head_node *cur_head);
    void insert(head_node *cur_head, node *cur);
    void update(node *cur);
};

template <class Key, class Value>
void LFUCache<Key, Value>::erase(LFUCache<Key, Value>::head_node *cur_head)
{
    cur_head->prev->next = cur_head->next;
    cur_head->next->prev = cur_head->prev;
    node *cur = cur_head->cur, *temp;
    delete cur_head;
    while (cur)
    {
        size--;
        temp = cur->next;
        map.erase(cur->key);
        delete cur;
        cur = temp;
    }
}

template <class Key, class Value>
void LFUCache<Key, Value>::insert(LFUCache<Key, Value>::head_node *cur_head, LFUCache<Key, Value>::node *cur)
{
    cur->head = cur_head;
    cur->prev = nullptr;
    cur->next = cur_head->cur;
    if (cur->next)
    {
        cur->next->prev = cur;
    }
    cur_head->cur = cur;
}

template <class Key, class Value>
void LFUCache<Key, Value>::update(LFUCache<Key, Value>::node *cur)
{
    head_node *cur_head = cur->head;
    if (cur->prev)
    {
        cur->prev->next = cur->next;
        if (cur->next)
        {
            cur->next->prev = cur->prev;
        }
    }
    else
    {
        cur_head->cur = cur->next;
        if (cur->next)
        {
            cur->next->prev = nullptr;
        }
    }
    head_node *next_head;
    if (cur_head->next->count == cur_head->count + 1)
    {
        next_head = cur_head->next;
    }
    else
    {
        next_head = new head_node(cur_head->count + 1);
        next_head->next = cur_head->next;
        next_head->next->prev = next_head;
        next_head->prev = cur_head;
        cur_head->next = next_head;
    }
    insert(next_head, cur);
}

template <class Key, class Value>
bool LFUCache<Key, Value>::get(Key k, Value &res)
{
    auto item = map.find(k);
    if (item == map.end())
        return false;
    node *cur = item->second;
    res = cur->val;
    update(cur);
    return true;
}

template <class Key, class Value>
void LFUCache<Key, Value>::put(Key k, Value v)
{
    auto item = map.find(k);
    if (item == map.end())
    {
        if (size >= capacity)
        {
            erase(head->next);
        }
        node *cur = new node(k, v);
        map[k] = cur;
        head_node *cur_head;
        if (head->next->count == 1)
        {
            cur_head = head->next;
        }
        else
        {
            cur_head = new head_node(1);
            cur_head->next = head->next;
            cur_head->prev = head;
            cur_head->next->prev = cur_head;
            head->next = cur_head;
        }
        insert(cur_head, cur);
        size++;
    }
    else
    {
        node *cur = item->second;
        update(cur);
        cur->val = v;
    }
}

#endif
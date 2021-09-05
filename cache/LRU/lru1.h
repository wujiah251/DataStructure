#ifndef LRU1_H_
#define LRU1_H_

#include <unordered_map>
#include <list>
#include <utility>
#include <iostream>
#include "base.h"

using std::cout;
using std::endl;
using std::list;
using std::pair;
using std::unordered_map;

template <typename Key, typename Value>
class LRUCache : public LRUBase<Key, Value>
{
private:
    typedef pair<Key, Value> node;
    list<node> list_;
    unordered_map<Key, typename list<node>::iterator> map_;
    size_t cap_;

public:
    LRUCache(size_t cap);
    ~LRUCache();
    void put(const Key &key, const Value &val);
    bool get(const Key &key, Value &res);
    void remove(const Key &key);
    size_t size() { return list_.size(); }
    void dump();
};

template <typename Key, typename Value>
LRUCache<Key, Value>::LRUCache(size_t cap) : cap_(cap)
{
}

template <typename Key, typename Value>
LRUCache<Key, Value>::~LRUCache()
{
}

template <typename Key, typename Value>
void LRUCache<Key, Value>::put(const Key &key, const Value &val)
{
    auto it = map_.find(key);
    if (it != map_.end())
    {
        list_.erase(it->second);
    }
    node n{key, val};
    list_.push_front(n);
    map_[key] = list_.begin();
    if (list_.size() > cap_)
    {
        auto item = list_.rbegin();
        map_.erase(item->first);
        list_.pop_back();
    }
}

template <typename Key, typename Value>
bool LRUCache<Key, Value>::get(const Key &key, Value &res)
{
    auto it = map_.find(key);
    if (it == map_.end())
        return false;
    auto item = it->second;
    node n{item->first, item->second};
    list_.push_front(n);
    list_.erase(item);
    map_[key] = list_.begin();
    return true;
}

template <typename Key, typename Value>
void LRUCache<Key, Value>::remove(const Key &key)
{
    auto item = map_.find(key);
    if (item == map_.end())
        return;
    auto it = item->second;
    list_.erase(it);
    map_.erase(key);
}

template <typename Key, typename Value>
void LRUCache<Key, Value>::dump()
{
    for (auto &item : list_)
    {
        cout << item.first << ',' << item.second << endl;
    }
}

#endif
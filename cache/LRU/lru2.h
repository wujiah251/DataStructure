#ifndef LRU2_H_
#define LRU2_H_

#include "base.h"
#include <unordered_map>
using std::unordered_map;

template <typename Key, typename Value>
class LRUCache : public LRUBase<Key, Value>
{
public:
    LRUCache();
    ~LRUCache();
    void put(const Key &key, const Value &val);
    bool get(const Key &key, Value &res);
    void remove(const Key &key);
};

#endif
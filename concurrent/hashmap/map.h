#ifndef MAP_H_
#define MAP_H_

#include <cstddef>

template <class Key, class Value>
class Map
{
public:
    virtual ~Map() {}
    virtual void put(const Key &key, const Value &val) = 0;
    virtual bool get(const Key &key, Value &res) = 0;
    virtual void remove(const Key &key) = 0;
};

#endif
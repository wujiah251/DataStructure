#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <functional>

template <class Key, class Value>
class HashMap
{
public:
    HashMap();
    virtual ~HashMap() = 0;
    virtual void put(const Key &key, const Value &val) = 0;
    virtual bool get(const Key &key, Value &res) = 0;
    virtual void remove(const Key &key) = 0;
    virtual size_t size() = 0;
};

template <typename Key, typename Value,
          typename Hash = std::hash<Key>,
          typename Equal = std::equal_to<Key>>
class HashMap1 : public HashMap<Key, Value>
// 互斥锁实现
{
private:
    size_t size_;
    Hash hash;
    Equal equal;

public:
    HashMap1();
    ~HashMap1();
    void put(const Key &key, const Value &val);
    bool get(const Key &key, Value &res);
    void remove(const Key &key);
    size_t size();
};

template <typename Key, typename Value, typename Hash, typename Equal>
HashMap1<Key, Value, Hash, Equal>::HashMap1()
{
}
template <typename Key, typename Value, typename Hash, typename Equal>
HashMap1<Key, Value, Hash, Equal>::~HashMap1()
{
}
template <typename Key, typename Value, typename Hash, typename Equal>
void HashMap1<Key, Value, Hash, Equal>::put(const Key &key, const Value &val)
{
}
template <typename Key, typename Value, typename Hash, typename Equal>
bool HashMap1<Key, Value, Hash, Equal>::get(const Key &key, Value &res)
{
}
template <typename Key, typename Value, typename Hash, typename Equal>
void HashMap1<Key, Value, Hash, Equal>::remove(const Key &key)
{
}
template <typename Key, typename Value, typename Hash, typename Equal>
size_t HashMap1<Key, Value, Hash, Equal>::size()
{
}

#endif
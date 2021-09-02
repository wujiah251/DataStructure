#ifndef SHARED_HASHMAP_H
#define SHARED_HASHMAP_H

#include <cstddef>
#include <limits.h>
#include <vector>
#include "hashmap.h"
using std::vector;

template <typename Key, typename Value,
          typename ZoneHash,
          typename Hash = std::hash<Key>>
class SharedHashMap : public Map<Key, Value>
{
private:
    typedef HashMap<Key, Value, Hash> hashmap;
    size_t zones_;
    vector<hashmap> shared_maps_;
    ZoneHash zone_hash_;
    Hash hash_;
    size_t get_zone(const Key &key)
    {
        return zone_hash_(key) & (zones_ - 1);
    }

public:
    SharedHashMap(int zones, double load_factor = 0.75)
    {
        zones_ = 4;
        while (zones_ < zones && zones_ * 2 < INT_MAX)
        {
            zones_ *= 2;
        }
        shared_maps_ = vector<hashmap>(zones_);
    }
    ~SharedHashMap() {}
    void put(const Key &key, const Value &val);
    bool get(const Key &key, Value &res);
    void remove(const Key &key);
    size_t size();
};

template <typename Key, typename Value,
          typename ZoneHash,
          typename Hash>
void SharedHashMap<Key, Value, ZoneHash, Hash>::put(const Key &key, const Value &val)
{
    shared_maps_[get_zone(key)].put(key, val);
}

template <typename Key, typename Value,
          typename ZoneHash,
          typename Hash>
bool SharedHashMap<Key, Value, ZoneHash, Hash>::get(const Key &key, Value &res)
{
    return shared_maps_[get_zone(key)].get(key, res);
}

template <typename Key, typename Value,
          typename ZoneHash,
          typename Hash>
void SharedHashMap<Key, Value, ZoneHash, Hash>::remove(const Key &key)
{
    return shared_maps_[get_zone(key)].remove(key);
}

#endif
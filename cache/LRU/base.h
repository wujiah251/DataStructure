
template <class Key, class Value>
class LRUBase
{
public:
    virtual ~LRUBase() {}
    virtual void put(const Key &key, const Value &val) = 0;
    virtual bool get(const Key &key, Value &res) = 0;
    virtual void remove(const Key &key) = 0;
};
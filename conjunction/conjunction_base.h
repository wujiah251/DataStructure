#ifndef _CONJUNCTION_BASE_H_
#define _CONJUNCTION_BASE_H_

#include <vector>
#include <functional>

template <class T>
class Conjunction
{
public:
    Conjunction() {}
    virtual ~Conjunction() {}
    virtual bool intersection(std::vector<std::vector<T>> &multi_list, std::vector<T> &res) = 0;

private:
};

#endif

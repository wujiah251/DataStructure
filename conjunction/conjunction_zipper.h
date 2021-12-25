#ifndef _CONJUNCTION_ZIPPER_H_
#define _CONJUNCTION_ZIPPER_H_

#include "conjunction_base.h"

// 拉链法
template <class T, class Comp = std::less<T>>
class ConjunctionZipper : public Conjunction<T>
{
public:
    ConjunctionZipper() {}
    ~ConjunctionZipper() {}
    virtual bool intersection(std::vector<std::vector<T>> &multi_set, std::vector<T> &res) override;
};

template <class T, class Comp>
bool ConjunctionZipper<T, Comp>::intersection(std::vector<std::vector<T>> &multi_set, std::vector<T> &res)
{
    if (multi_set.size() < 1)
        return false;
    if (multi_set.size() == 1)
    {
        res = multi_set[0];
        return true;
    }
    if (multi_set.size() > 2)
        return false;
    size_t index1 = 0, index2 = 0;
    std::vector<T> &set1 = multi_set[0];
    std::vector<T> &set2 = multi_set[1];
    res.clear();
    while (index1 < multi_set[0].size() && index2 < multi_set[1].size())
    {

        if (set1[index1] == set2[index2])
        {
            // ==
            res.push_back(set1[index1++]);
            ++index2;
        }
        else if (set1[index1] < set2[index2])
        {
            ++index1;
        }
        else
        {
            ++index2;
        }
    }
    return true;
}
#endif
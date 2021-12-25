#ifndef _CONJUNCTION_TRAVERSE_H_
#define _CONJUNCTION_TRAVERSE_H_

// 遍历法
#include <limits.h>
#include "conjunction_base.h"

template <class T>
class ConjunctionTraverse : public Conjunction<T>
{
public:
    ConjunctionTraverse() {}
    ~ConjunctionTraverse() override {}
    bool intersection(std::vector<std::vector<T>> &multi_list, std::vector<T> &res) override;
};

template <class T>
bool ConjunctionTraverse<T>::intersection(std::vector<std::vector<T>> &multi_set, std::vector<T> &res)
{
    if (multi_set.size() < 1)
        return false;
    if (multi_set.size() == 1)
    {
        res = multi_set[0];
        return true;
    }
    size_t N = multi_set.size();
    int index = -1, len = INT_MAX;
    for (int i = 0; i < multi_set.size(); ++i)
    {
        if (multi_set[i].size() < len)
        {
            index = i;
            len = multi_set[i].size();
        }
    }
    multi_set[index].swap(multi_set[0]);
    res.clear();
    for (int i = 0; i < multi_set[0].size(); ++i)
    {
        res.push_back(multi_set[0][i]);
        for (int j = 1; j < multi_set.size(); ++j)
        {
            bool flag = false;
            for (int k = 0; k < multi_set[j].size(); ++k)
            {
                if (multi_set[j][k] == multi_set[0][i])
                {
                    flag = true;
                    break;
                }
            }
            if (!flag)
            {
                res.pop_back();
                break;
            }
        }
    }
    return true;
}
#endif

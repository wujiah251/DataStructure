#ifndef _CONJUNCTION_H_
#define _CONJUNCTION_H_

#include <limits.h>
#include <algorithm>
#include "conjunction_base.h"

template <class T>
class ConjunctionSkip : public Conjunction<T>
{
public:
    ConjunctionSkip() {}
    ~ConjunctionSkip() {}
    bool intersection(std::vector<std::vector<T>> &multi_set, std::vector<T> &res) override;
};

template <class T>
bool ConjunctionSkip<T>::intersection(std::vector<std::vector<T>> &multi_set, std::vector<T> &res)
{
    if (multi_set.size() < 1)
        return false;
    if (multi_set.size() == 1)
    {
        res = multi_set[0];
        return true;
    }
    size_t N = multi_set.size();
    int index = 0, len = INT_MAX;
    for (int i = 0; i < N; ++i)
    {
        if (multi_set[i].size() < len)
        {
            index = i;
            len = multi_set[i].size();
        }
    }
    multi_set[index].swap(multi_set[0]);
    std::vector<typename std::vector<T>::const_iterator> itor_vec(N);
    for (int j = 1; j < N; ++j)
        itor_vec[j] = multi_set[j].begin();
    res.clear();
    for (int i = 0; i < multi_set[0].size(); ++i)
    {
        bool flag = true;
        for (int j = 1; j < N; ++j)
        {
            auto itor = std::lower_bound(itor_vec[j], multi_set[j].end(), multi_set[0][i]);
            if (itor == multi_set[j].end())
            {
                flag = false;
                itor_vec[j] = itor;
            }
            else if (*itor == multi_set[0][i])
            {
                itor_vec[j] = itor + 1;
            }
            else
            {
                itor_vec[j] = itor;
                flag = false;
            }
        }
        if (flag)
        {
            // 每个list都存在的值
            res.push_back(multi_set[0][i]);
        }
    }
    return true;
}

#endif
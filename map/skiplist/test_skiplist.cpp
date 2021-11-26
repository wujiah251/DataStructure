#include <iostream>
#include <sstream>
#include <assert.h>
#include "skiplist.h"
using namespace std;

string int2str(int n)
{
    stringstream ss;
    ss << n;
    return ss.str();
}

int main()
{
    std::cout << "start test------------" << std::endl;
    int a;
    string b;
    SkipList<int, string> map(4);
    for (int i = 20; i >= 0; i -= 4)
    {
        map.put(i, int2str(i));
    }
    std::cout << "passed one------------" << std::endl;
    assert(map.size() == 6);
    map.print();
    for (int i = 20; i >= 0; i -= 4)
    {
        cout << i << endl;
        assert(map.get(i, b));
        assert(b == int2str(i));
    }
    std::cout << "passed two------------" << std::endl;
    for (int i = 20; i >= 0; i -= 2)
    {
        map.put(i, int2str(i * i));
    }
    assert(map.size() == 11);
    for (int i = 20; i >= 0; i -= 2)
    {
        assert(map.get(i, b));
        assert(b == int2str(i * i));
    }
    for (int i = 20; i >= 0; i -= 1)
    {
        map.put(i, int2str(i * i * i));
    }
    assert(map.size() == 21);
    for (int i = 20; i >= 0; i -= 1)
    {
        assert(map.get(i, b));
        assert(b == int2str(i * i * i));
    }
    map.print();
    std::cout << "pass test-------------" << std::endl;
    return 0;
}
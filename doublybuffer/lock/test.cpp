#include <thread>
#include <map>
#include <vector>
#include <assert.h>
#include <functional>
#include <ctime>
#include <string>
#include <chrono>
#include <sstream>
#include "doubly_buffer.h"
using namespace std;

typedef map<int, int> Map;

struct Test
{
  DoublyBuffer<Map> buffer;
  vector<thread> readers;
  vector<thread> writers;
  int num_max;
  int write_times;
  Test(int read_nums, int write_nums, int num_max) : readers(read_nums), writers(write_nums), num_max(num_max) {}
  static void read(DoublyBuffer<Map> *buffer, int num_max)
  {
    for (int i = 0; i < num_max; ++i)
    {
      DoublyBuffer<Map>::ScopedPtr ptr;
      if (buffer->Read(&ptr))
      {
        const Map &data = *(ptr.get());
        if (data.find(i) != data.end())
        {
          assert(i == data.at(i));
        }
        // stringstream ss;
        // ss << this_thread::get_id();
        // string str = "thread[" + ss.str() + "] read num " + to_string(i) + '\n';
        // cout << str;
      }
      else
      {
        cout << "Read Error" << endl;
      }
    }
  }
  static bool write_num(Map &map, int i)
  {
    pair<Map::iterator, bool> flag = map.insert(make_pair(i, i));
    return flag.second;
  }
  static void write(DoublyBuffer<Map> *buffer, int num_max)
  {
    for (int i = 0; i < num_max; ++i)
    {
      auto fn = bind(write_num, placeholders::_1, i);
      // stringstream ss;
      // ss << this_thread::get_id();
      // string str = "thread[" + ss.str() + "] write num " + to_string(i) + '\n';
      // cout << str;
      if (!buffer->Modify(fn))
      {
        cout << "Modify " << i << " error" << endl;
      }
    }
  }
  void run()
  {
    auto start = chrono::system_clock::now();
    for (int i = 0; i < writers.size(); ++i)
    {
      writers[i] = thread(bind(write, &buffer, num_max));
    }
    for (int i = 0; i < readers.size(); ++i)
    {
      readers[i] = thread(bind(read, &buffer, num_max));
    }
    for (int i = 0; i < writers.size(); ++i)
    {
      writers[i].join();
    }
    for (int i = 0; i < readers.size(); ++i)
    {
      readers[i].join();
    }
    auto end = chrono::system_clock::now();
    cout << "cost time:" << (end - start).count() << endl;
    // check
    for (int i = 0; i < num_max; ++i)
    {
      DoublyBuffer<Map>::ScopedPtr ptr;
      buffer.Read(&ptr);
      auto &map = *(ptr.get());
      assert(map.at(i) == i);
    }
  }
};

int main()
{
  Test test(1, 1, 10);
  test.run();
  return 0;
}
#include <thread>
#include <map>
#include <vector>
#include <assert.h>
#include <functional>
#include <ctime>
#include <string>
#include <chrono>
#include <sstream>
#include <iostream>
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
    int i = 1;
    while (i <= num_max)
    {
      auto ptr = buffer->Read();
      if (ptr->find(i) != ptr->end())
      {
        assert(ptr->at(i) == i);
        // stringstream ss;
        // ss << this_thread::get_id();
        // string str = "Thread[" + ss.str() + "] Read num " + to_string(i) + '\n';
        // cout << str;
      }
      i++;
    }
  }
  static bool write_num(Map &map, int i)
  {
    pair<Map::iterator, bool> flag = map.insert(make_pair(i, i));
    return flag.second;
  }
  static void write(DoublyBuffer<Map> *buffer, int num_max)
  {
    int i = 1;
    while (i <= num_max)
    {
      auto fn = bind(write_num, placeholders::_1, i);
      if (!buffer->Write(fn))
        cout << "Write Error num " << i << endl;
      else
      {
        // stringstream ss;
        // ss << this_thread::get_id();
        // string str = "Thread[" + ss.str() + "] Write num " + to_string(i) + '\n';
        // cout << str;
      }
      i++;
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
    auto ptr = buffer.Read();
    for (int i = 1; i <= num_max; ++i)
    {
      assert(ptr->at(i) == i);
    }
  }
};

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    cout << "Please input num of readers and writers, num_max" << endl;
    return 0;
  }
  int reader_num = atoi(argv[1]);
  int writers_num = atoi(argv[2]);
  int num_max = atoi(argv[3]);
  Test test(reader_num, writers_num, num_max);
  test.run();
  return 0;
}
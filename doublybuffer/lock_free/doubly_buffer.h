#include <memory>
#include <mutex>
#include <atomic>

using std::atomic_int;
using std::lock_guard;
using std::make_shared;
using std::memory_order_acquire;
using std::memory_order_relaxed;
using std::memory_order_release;
using std::mutex;
using std::shared_ptr;

template <class T>
class DoublyBuffer
{
public:
  DoublyBuffer() : _index(0)
  {
    _data[0] = make_shared<T>();
    _data[1] = make_shared<T>();
  }
  ~DoublyBuffer() {}
  DoublyBuffer(const DoublyBuffer &other) = delete;
  DoublyBuffer &operator=(const DoublyBuffer &other) = delete;
  shared_ptr<T> Read();
  template <class Fn>
  bool Write(Fn &fn);

private:
  shared_ptr<T> _data[2];
  atomic_int _index;
  mutex _write_mux;
};

template <class T>
shared_ptr<T> DoublyBuffer<T>::Read()
{
  return _data[_index.load(memory_order_acquire)];
}

template <class T>
template <class Fn>
bool DoublyBuffer<T>::Write(Fn &fn)
{
  lock_guard<mutex> guard(_write_mux);
  // 先写备份数据
  // 写
  int bg_index = 1 - _index.load(memory_order_relaxed);
  const bool ret = fn(*(_data[bg_index].get()));
  if (!ret)
  {
    return false;
  }
  _index.store(bg_index, memory_order_release);
  bg_index = 1 - bg_index;
  // 等待所有读线程退出
  while (_data[bg_index].use_count() > 1)
    ;
  const size_t ret2 = fn(*(_data[bg_index].get()));
  return true;
}

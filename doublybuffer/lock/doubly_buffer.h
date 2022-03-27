#ifndef DOUBLYBUFFER_H_
#define DOUBLYBUFFER_H_

#include <new>
#include <atomic>
#include <mutex>
#include <vector>
#include <thread>
#include <iostream>

template <typename T>
class DoublyBuffer
{
private:
  class Wrapper
  {
    friend class DoublyBuffer;

  private:
    static void destroy(void *arg)
    {
      delete static_cast<Wrapper *>(arg);
    }

  public:
    Wrapper(DoublyBuffer *c) : _control(c) {}
    ~Wrapper()
    {
      if (_control != nullptr)
      {
        _control->RemoveWrapper(this);
      }
    }
    void BeginRead()
    {
      _mutex.lock();
    }
    void EndRead()
    {
      _mutex.unlock();
    }
    void WaitReadDone()
    {
      // 获得锁再释放表示等待读者读完数据
      _mutex.lock();
      _mutex.unlock();
    }

  private:
    DoublyBuffer *_control;
    std::mutex _mutex;
  };

public:
  class ScopedPtr
  {
    friend class DoublyBuffer;

  public:
    ScopedPtr() : _data(nullptr), _w(nullptr) {}
    ~ScopedPtr()
    {
      if (_w)
      {
        _w->EndRead();
      }
    }
    const T *get() const { return _data; }
    const T &operator*() const { return *_data; }
    const T *operator->() const { return _data; }

  private:
    // 禁用复制构造函数和赋值函数
    ScopedPtr(const ScopedPtr &) = delete;
    ScopedPtr &operator=(const ScopedPtr &) = delete;
    const T *_data;
    Wrapper *_w;
  };

public:
  DoublyBuffer();
  ~DoublyBuffer();
  bool Read(ScopedPtr *ptr);
  template <typename Fn>
  bool Modify(Fn &fn);

private:
  std::atomic<int> _index;
  T _data[2];
  // 是否创建了线程本地存储
  bool _created_key;
  // 线程本地存储key
  pthread_key_t _wrapper_key;
  // 所有读者持有的wrapper
  std::vector<Wrapper *> _wrappers;
  std::mutex _wrappers_mutex;
  std::mutex _modify_mutex;

private:
  // 不安全读数据
  const T *UnsafeRead() const
  {
    return _data + _index.load(std::memory_order_acquire);
  }
  Wrapper *AddWrapper()
  {
    Wrapper *w = new (std::nothrow) Wrapper(this);
    if (w == nullptr)
    {
      return nullptr;
    }
    try
    {
      std::lock_guard<std::mutex> guard(_wrappers_mutex);
      _wrappers.push_back(w);
    }
    catch (std::exception &e)
    {
      return nullptr;
    }
    return w;
  }
  void RemoveWrapper(Wrapper *w)
  {
    if (w == nullptr)
    {
      return;
    }
    std::lock_guard<std::mutex> guard(_wrappers_mutex);
    for (size_t i = 0; i < _wrappers.size(); ++i)
    {
      if (_wrappers[i] == w)
      {
        _wrappers[i] = _wrappers.back();
        _wrappers.pop_back();
        return;
      }
    }
  }
};

template <typename T>
DoublyBuffer<T>::DoublyBuffer()
    : _index(0),
      _created_key(false),
      _wrapper_key(0)
{
  _wrappers.reserve(64);
  const int rc = pthread_key_create(&_wrapper_key,
                                    Wrapper::destroy);
  if (rc != 0)
  {
    std::cout << "Fail to pthread_key_create" << std::endl;
  }
  else
  {
    _created_key = true;
  }
  _data[0] = T();
  _data[1] = T();
}

template <typename T>
DoublyBuffer<T>::~DoublyBuffer()
{
  if (_created_key)
  {
    pthread_key_delete(_wrapper_key);
  }
  {
    std::lock_guard<std::mutex> guard(_wrappers_mutex);
    // 等待所有读者读完
    for (size_t i = 0; i < _wrappers.size(); ++i)
    {
      _wrappers[i]->_control = nullptr;
      delete _wrappers[i];
    }
    _wrappers.clear();
  }
}

template <typename T>
bool DoublyBuffer<T>::Read(ScopedPtr *ptr)
{
  if (!_created_key)
  {
    return false;
  }
  // 获取线程本地存储
  Wrapper *w = static_cast<Wrapper *>(pthread_getspecific(_wrapper_key));
  if (w != nullptr)
  {
    w->BeginRead();
    ptr->_data = UnsafeRead();
    ptr->_w = w;
    return true;
  }
  w = AddWrapper();
  if (w != nullptr)
  {
    // 设置线程本地存储
    const int rc = pthread_setspecific(_wrapper_key, w);
    if (rc == 0)
    {
      w->BeginRead();
      ptr->_data = UnsafeRead();
      ptr->_w = w;
      return true;
    }
  }
  return false;
}

template <typename T>
template <typename Fn>
bool DoublyBuffer<T>::Modify(Fn &fn)
{
  // 上锁保证至多一个写者
  std::lock_guard<std::mutex> modify_guard(_modify_mutex);
  int bg_index = 1 - _index.load(std::memory_order_relaxed);
  const bool ret = fn(_data[bg_index]);
  if (!ret)
  {
    return false;
  }
  // 更新完备份数据，更新index，这样新的读者会读取更新后的数据
  _index.store(bg_index, std::memory_order_release);
  bg_index = 1 - bg_index;
  {
    std::lock_guard<std::mutex> wrapper_guard(_wrappers_mutex);
    // 等待所有读取主份数据的读者结束
    for (size_t i = 0; i < _wrappers.size(); ++i)
    {
      _wrappers[i]->WaitReadDone();
    }
    const bool ret2 = fn(_data[bg_index]);
    if (!ret2)
      std::cout << "ret2=false" << std::endl;
    return ret2;
  }
}

#endif
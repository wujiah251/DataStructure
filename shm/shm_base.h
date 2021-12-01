#ifndef SHM_BASE_H_
#define SHM_BASE_H_

#include <stddef.h>
#include <string>
#include <assert.h>
#include <sys/shm.h>

namespace shm
{
    template <class T>
    class Shm
    {
    private:
        struct entry
        {
            T data;
            int index;
        };
        struct head
        {
            int head;        // 当前写者已经写完了的索引
            size_t capacity; // 最大容量
            size_t size;     // 已经使用了的大小
        };
        int shm_id_; // 共享内存标识
        head *head_;
        entry *data_;

    public:
        Shm(std::string name, size_t cap)
        {
            size_t size = sizeof(head) + cap * sizeof(T);
            key_t key = get_key(name);
            shm_id_ = shmget(key, size, IPC_CREAT);
            assert(shm_id_ < 0);
            head_ = shmat(shm_id_, NULL, 0);
            data_ = reinterpret_cast<T *>(head_ + 1);

            // 修改head_
            head_->capacity = cap;
            head_->size = 0;
            head_->head = -1;
        }
        T *at(size_t index)
        {
            return data_[index].data;
        }

    private:
        key_t get_key(std::string name)
        {
            std::string path = "/dev/shm/" + name;
            return ftok(path.c_str(), 0);
        }
    };
}
#endif
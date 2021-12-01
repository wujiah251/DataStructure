#ifndef SHM_WRITER_H_
#define SHM_WRITER_H_

#include <string>
#include <sys/shm.h>
#include <assert.h>
namespace shm
{
    template <typename T>
    class ShmWriter
    {
    public:
        ShmWriter(std::string name, size_t cap);
        ~ShmWriter();
        T *get_slot();        // 获取当前的slot
        void commit(T *slot); // 提交当前
    private:
        key_t get_key(std::string name)
        {
            std::string path = "/dev/shm/" + name;
            return ftok(path.c_str(), 0);
        }

    protected:
    private:
        struct head
        {
            int head;          // 当前写者已经写完了的索引
            size_t capacity;   // 最大容量
            size_t size;       // 已经使用了的大小
            size_t produce_id; // 生产者id
        };
        int shm_id_; // 共享内存标识
        head *head_;
        T *data_;
    };
    template <typename T>
    ShmWriter<T>::ShmWriter(std::string name, size_t cap)
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
}
#endif
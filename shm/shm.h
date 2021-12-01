#ifndef SHM_H_
#define SHM_H_

#include "shm_reader.h"
#include "shm_writer.h"

namespace shm
{
    template <typename T>
    struct shm_traits_t
    {
        typedef ShmReader<T> shm_reader;
        typedef ShmWriter<T> shm_writer;
    };
}
#endif
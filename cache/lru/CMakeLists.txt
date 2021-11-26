add_executable(TestLru test_lru.cpp)

install(TARGETS TestLru
    DESTINATION "test/"
)
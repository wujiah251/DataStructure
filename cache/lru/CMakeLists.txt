add_executable(TESTLRU test_lru.cpp)

install(TARGETS TESTLRU
    DESTINATION "test/"
)
add_executable(TestLfu test_lfu.cpp)

install(TARGETS TestLfu
    DESTINATION "test/"
)
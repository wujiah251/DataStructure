add_executable(TESTLFU test_lfu.cpp)

install(TARGETS TESTLFU
    DESTINATION "bin/"
)
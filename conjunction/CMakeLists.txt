add_executable(TestConjunction test_conjunction.cpp)

target_link_libraries(TestConjunction
    libbenchmark.a
    pthread
)

add_executable(BaseTestConjunction base_test_conjunction.cpp)

target_link_libraries(TestConjunction
    libbenchmark.a
    pthread
)

install(TARGETS
    TestConjunction
    BaseTestConjunction
    DESTINATION bin/
)
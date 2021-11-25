add_executable(TestObjPool obj_pool_test.cpp)

target_include_directories(TestObjPool PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/pool"
                          )


install(TARGETS TestObjPool
    DESTINATION "test/"
)
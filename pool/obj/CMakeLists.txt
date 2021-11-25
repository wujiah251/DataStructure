add_executable(ObjPoolTest obj_pool_test.cpp)

target_include_directories(ObjPoolTest PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/pool"
                          )


install(TARGETS ObjPoolTest
    DESTINATION "bin/"
)
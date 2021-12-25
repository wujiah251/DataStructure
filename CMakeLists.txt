cmake_minimum_required(VERSION 3.10)

# 设定项目名称
project(DataStruct)

# C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-pg")
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_BUILD_TYPE "Debug")


add_subdirectory(cache)
add_subdirectory(concurrent)
add_subdirectory(map)
add_subdirectory(pool)
add_subdirectory(conjunction)

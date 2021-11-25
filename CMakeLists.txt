cmake_minimum_required(VERSION 3.10)

# 设定项目名称
project(DataStruct)

# C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/build)

add_subdirectory(pool)

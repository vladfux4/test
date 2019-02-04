cmake_minimum_required(VERSION 2.8)
project(test_project)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --std=c++17")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_LIST_DIR}/cmake"
    CACHE STRING "Modules for CMake" FORCE)
find_package(Boost 1.60 REQUIRED system thread)
find_package(glog 0.3.5 REQUIRED)
find_package(gflags REQUIRED)

#set(CMAKE_BUILD_TYPE Debug)

include_directories(${Boost_INCLUDE_DIR} .)

add_executable(${PROJECT_NAME}
    "static_pool_alloc.h"
    "block_pool.h"
    "worker.h"
    "consumer_worker.h"
    "generator_worker.h"
    "generator_worker.cc"
    "scheduler_worker.h"
    "scheduler_worker.cc"
    "shared_block.h"
    "compute_worker.h"
    "compute_worker.cc"
    "crc32_calculator.h"
    "controller.h"
    "controller.cc"
    "main.cc"
    )

target_link_libraries(${PROJECT_NAME}
    LINK_PUBLIC ${Boost_LIBRARIES} glog gflags pthread)

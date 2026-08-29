cmake_minimum_required(VERSION 3.10)
project(video)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

message(STATUS "OpenCV version: ${OpenCV_VERSION}")

# 运动检测系统 (帧差法/MOG2 + 多线程 + 录制)
add_executable(motion_detect motion_detect.cpp)
target_link_libraries(motion_detect PRIVATE ${OpenCV_LIBS})

# 条码检测 (示例)
add_executable(f barcode.cpp)
target_link_libraries(f PRIVATE ${OpenCV_LIBS})

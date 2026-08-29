cmake_minimum_required(VERSION 3.10)
project(video-motion-detection-system
    VERSION 1.0.0
    DESCRIPTION "Real-time Video Capture and Motion Detection System"
    LANGUAGES CXX
)

# C++14 (std::thread / mutex / condition_variable)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -O2)
endif()

# 查找 OpenCV
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

# 头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 源文件
set(SOURCES
    src/main.cpp
    src/frame_queue.cpp
    src/motion_detector.cpp
    src/video_recorder.cpp
)

# 可执行文件
add_executable(motion_detect ${SOURCES})
target_link_libraries(motion_detect PRIVATE ${OpenCV_LIBS})

# 安装规则
install(TARGETS motion_detect DESTINATION bin)

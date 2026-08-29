cmake_minimum_required(VERSION 3.10)
project(video-broadcast-linux
    VERSION 1.0.0
    DESCRIPTION "One-way Video Broadcast System on Linux"
    LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 编译选项
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -O2)
endif()

# 头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 查找依赖
# FFmpeg
find_package(PkgConfig REQUIRED)
pkg_check_modules(AVCODEC REQUIRED IMPORTED_TARGET libavcodec)
pkg_check_modules(AVUTIL REQUIRED IMPORTED_TARGET libavutil)
pkg_check_modules(AVFORMAT REQUIRED IMPORTED_TARGET libavformat)

# ALSA (可选, 没装也能编译)
pkg_check_modules(ALSA IMPORTED_TARGET alsa)
if(ALSA_FOUND)
    add_compile_definitions(HAVE_ALSA=1)
    message(STATUS "ALSA: found (version ${ALSA_VERSION})")
else()
    message(STATUS "ALSA: not found (音频采集模块将不可用)")
endif()

# Qt6 (可选, 预留 GUI 模块)
# find_package(Qt6 COMPONENTS Core Gui Network Widgets)

# 源文件
set(SOURCES
    src/main.c
    src/common/frame_pool.c
    src/common/lock_free_queue.c
    src/capture/video_capture.c
    src/capture/audio_capture.c
    src/codec/video_encoder.c
    src/codec/audio_encoder.c
    src/transport/rtp_packetizer.c
    src/transport/multicast_sender.c
    src/signaling/signaling_server.c
)

# 可执行文件: 发送端
add_executable(broadcast_sender ${SOURCES})
target_link_libraries(broadcast_sender PRIVATE
    PkgConfig::AVCODEC
    PkgConfig::AVUTIL
    PkgConfig::AVFORMAT
    pthread
)

if(ALSA_FOUND)
    target_link_libraries(broadcast_sender PRIVATE PkgConfig::ALSA)
endif()

# 安装
install(TARGETS broadcast_sender DESTINATION bin)

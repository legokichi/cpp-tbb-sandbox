cmake_minimum_required(VERSION 3.5.1)
project(cpp-tbb-sandbox)

# option
set(CMAKE_C_COMPILER gcc-7)
set(CMAKE_CXX_COMPILER g++-7)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -v -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# -fgnu-tm - transactional memory

# append cmake find modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMakeModules/")

# pthread
#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)

# TBB
#find_package(TBB COMPONENTS tbb)

# OpenSSL
find_package(OpenSSL REQUIRED)

# OpenSSL
find_package(OpenSSL REQUIRED)

# boost
#set(Boost_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/local/include)
#set(Boost_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/local/lib)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.66.0 COMPONENTS system program_options context coroutine regex thread)
#find_package(Boost REQUIRED COMPONENTS system program_options context coroutine regex thread)
find_package(cppnetlib 0.11 REQUIRED COMPONENTS client-connections server-parsers)
set(CPPNETLIB_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/third_party/cpp-netlib/deps/uri/include/)


# FFMpeg
#find_package(FFmpeg REQUIRED)


# OpenCV
#find_package(OpenCV REQUIRED)

# WebRTC
#include(CMakeHelpers REQUIRED)
#include(CMakeFindExtensions REQUIRED)
#set(WEBRTC_ROOT_DIR ${PROJECT_SOURCE_DIR}/third_party/webrtc-checkout/src)
#set(WEBRTC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/third_party/webrtc-checkout/src)
#find_package(WebRTC REQUIRED)





# build
file(GLOB_RECURSE SOURCE_FILES "./src/*.cpp")
add_executable(
    main
    ${SOURCE_FILES}
)

# minitrace ( need -lpthread -lm )
#include_directories(${PROJECT_SOURCE_DIR}/third_party/minitrace/)
#target_sources(main PUBLIC ${PROJECT_SOURCE_DIR}/third_party/minitrace/minitrace.c)


include_directories(
    ${PROJECT_SOURCE_DIR}/local/include
    #${WEBRTC_INCLUDE_DIR}
    #${OPENCV_INCLUDE_DIRS}
    #${FFMPEG_INCLUDE_DIR}
    #${TBB_INCLUDE_DIR}
    ${CPPNETLIB_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(
    main
    boost_coroutine
    ${PROJECT_SOURCE_DIR}/local/lib
    #${WEBRTC_LIBRARIES}
    #${OpenCV_LIBS}
    #${FFMPEG_LIBRARIES}
    #${TBB_LIBRARIES}
    ${CPPNETLIB_LIBRARIES}
    ${Boost_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    #${CMAKE_THREAD_LIBS_INIT}
)

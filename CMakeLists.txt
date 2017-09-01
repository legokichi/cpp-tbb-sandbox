cmake_minimum_required(VERSION 3.5.1)
project(cpp-tbb-sandbox)

# option
set(CMAKE_C_COMPILER gcc-7)
set(CMAKE_CXX_COMPILER g++-7)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -v -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -fgnu-tm")

# append modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMakeModules/")


# pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


# TBB
find_package(TBB COMPONENTS tbb tbbmalloc tbbmalloc_proxy)


# boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.58.0 COMPONENTS program_options regex thread filesystem)


# FFMpeg
find_package(FFmpeg REQUIRED)


# OpenCV
#find_package(OpenCV REQUIRED)


# build
file(GLOB_RECURSE SOURCE_FILES "./src/*.cpp")
include_directories(
#    ${PROJECT_SOURCE_DIR}
#    ${OPENCV_INCLUDE_DIRS}
    ${FFMPEG_INCLUDE_DIR}
    ${TBB_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)
add_executable(
    main
    ${SOURCE_FILES}
)
target_link_libraries(
    main
#    ${OpenCV_LIBS}
    ${FFMPEG_LIBRARIES}
    ${TBB_LIBRARIES}
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

#add_subdirectory(foo)
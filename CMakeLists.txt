cmake_minimum_required(VERSION 3.5.1)
project(cpp-tbb-sandbox)

# option
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -v -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fgnu-tm")


# pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


# TBB
list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/FindTBB/")
find_package(TBB COMPONENTS tbbmalloc tbbmalloc_proxy)

# boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.58.0 COMPONENTS program_options regex thread filesystem)


# OpenCV
find_package(OpenCV REQUIRED)


# build
file(GLOB_RECURSE SOURCE_FILES "./src/*.cpp")
include_directories(
    ${OPENCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}
)
link_directories(
    ${OPENCV_LIBRARY_DIRS}
    ${BOOST_LIBRARY_PATH}
)
add_executable(
    main
    ${SOURCE_FILES}
)
target_link_libraries(
    main
    tbb
    ${OpenCV_LIBS}
    boost_coroutine
    ${Boost_LIBRARIES}
    Threads::Threads
    pthread
)

#add_subdirectory(foo)


cmake_minimum_required(VERSION 3.0.0)
project(1video VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(1video 1video.cpp)
target_link_libraries(1video ${OpenCV_LIBS})


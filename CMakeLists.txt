cmake_minimum_required(VERSION 3.20)
project(OpenCVBasic)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(main src/main.cpp)
target_link_libraries(main ${OpenCV_LIBS})

# set_property(TARGET main PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})










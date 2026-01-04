cmake_minimum_required(VERSION 3.13)

project(shell-starter-cpp)

include_directories(include)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

file(GLOB_RECURSE HEADER_FILES include/**.cpp include/**.h)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

add_executable(shell ${SOURCE_FILES} ${HEADER_FILES})

# target_link_libraries(shell PRIVATE readline)

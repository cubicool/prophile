project(prophile)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(prophile-c "prophile.c" "examples/prophile-c.c")
# add_executable(prophile-cpp "prophile.c" "examples/prophile-cpp.cpp")

if(WIN32)
	target_compile_options(prophile-c PRIVATE /W4 /wd4100)
else(WIN32)
	target_compile_options(prophile-c PRIVATE -W -Wall -Wno-unused-parameter)
endif()

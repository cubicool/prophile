project(prophile)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 99)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(prophile-typical "prophile.c" "examples/prophile-typical.c")
add_executable(prophile-ticks "prophile.c" "examples/prophile-ticks.c")

if(WIN32)
	target_compile_options(prophile-typical PRIVATE /W4 /wd4100)
	target_compile_options(prophile-ticks PRIVATE /W4 /wd4100)
else(WIN32)
	target_compile_options(prophile-typical PRIVATE -W -Wall -Wno-unused-parameter)
	target_compile_options(prophile-ticks PRIVATE -W -Wall -Wno-unused-parameter)
endif()

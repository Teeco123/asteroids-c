cmake_minimum_required(VERSION 3.9)
project(Asteroids)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT USE_SYSTEM_RAYLIB)
	set(BUILD_EXAMPLES OFF)
	add_subdirectory(raylib)
endif()

file(GLOB cSource src/*.c)
add_executable(Asteroids ${cSource})
target_link_libraries(Asteroids PRIVATE raylib m)
target_include_directories(Asteroids PRIVATE include/)

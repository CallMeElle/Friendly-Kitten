cmake_minimum_required(VERSION 3.17)

project("Friendly Kitten")

SET(OUTPUT "kittens")

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -g -Wall")

SET(GCC_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(
    ${OUTPUT} 
    src/game.cpp
    src/main.cpp)

find_library(GLFW
    NAMES libglfw.so.3 libglfw.so glfw libglfw.so.3.2
    HINTS /usr/lib/x86_64-linux-gnu/ ${CMAKE_PREFIX_PATH}/deploy/)

target_link_libraries(${OUTPUT} ${GLFW})
target_link_libraries(${OUTPUT} dl)

find_library(REDHAND
    NAMES libredhand.so redhand)   

target_link_libraries(${OUTPUT} ${REDHAND})

cmake_minimum_required(VERSION 3.17)

project("Friendly Kitten")

option(REPOROOT "Path of the root of the repo")
option(REDHAND_LOCATION "The exact locataion of the libredhand file relative to reporoot")

#set the reporoot var
if (REPOROOT)
    SET(CMAKE_PREFIX_PATH ${REPOROOT})
else (REPOROOT)
    SET(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR}/..)
endif (REPOROOT)

#include the headers
include_directories(${CMAKE_PREFIX_PATH}/include/)
include_directories(${CMAKE_PREFIX_PATH}/../redhand/include/)

#search for opengl
find_package(OpenGL REQUIRED)

#search for glfw
find_package(glfw3 REQUIRED)

#search for glib and vips
find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_check_modules(VIPS vips-cpp REQUIRED)

#Search for filesystem header
include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX("filesystem" HAVE_FILESYSTEM)

if(!HAVE_FILESYSTEM)
    message( FATAL_ERROR "compiler does not have c++ <filesystem> support please install a newer version." )
endif()

include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX("filesystem" HAVE_FILESYSTEM)

# Set output compiler flags
SET(OUTPUT "kittens")

if (UNIX)
    SET(GCC_COVERAGE_LINK_FLAGS "-pthread -ldl -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama -lX11")
endif (UNIX)

if (WIN32)
    SET(GCC_COVERAGE_LINK_FLAGS "-pthread")
endif (WIN32)

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -g -O0 -Wall -Wextra")
SET(GC_COVERAGE_COMPILE_FLAGS "-g -O0")

SET(GCC_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(GCC_C_FLAGS  "${CMAKE_C_FLAGS} ${GC_COVERAGE_COMPILE_FLAGS}")
SET(GCC_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

# add the source files
add_executable(
    ${OUTPUT} 
    src/game.cpp
    src/main.cpp
)

#include glib and link libraries
include_directories(${OUTPUT} ${GLIB_INCLUDE_DIRS})
target_link_libraries(${OUTPUT} ${GLIB_LDFLAGS})
target_link_libraries(${OUTPUT} OpenGL::GL)
target_link_libraries(${OUTPUT} glfw)
include_directories(${OUTPUT} ${VIPS_INCLUDE_DIRS})
target_link_libraries(${OUTPUT} ${VIPS_LDFLAGS})

target_link_libraries(${OUTPUT} dl)

#find and link redhand
if(REDHAND_LOCATION)
    message("using custom REDHAND_LOCATION")
    target_link_libraries(${OUTPUT} ${CMAKE_PREFIX_PATH}/${REDHAND_LOCATION})
else()
    find_library(REDHAND
        NAMES libredhand.so redhand
        HINTS "${CMAKE_PREFIX_PATH}/lib/" "${CMAKE_PREFIX_PATH}/deploy/" "${CMAKE_PREFIX_PATH}" "${CMAKE_PREFIX_PATH}/../redhand" "${CMAKE_PREFIX_PATH}/../redhand/deploy" "${CMAKE_PREFIX_PATH}/../redhand/lib")    

    target_link_libraries(${OUTPUT} ${REDHAND})
endif(REDHAND_LOCATION)  


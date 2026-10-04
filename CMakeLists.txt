cmake_minimum_required(VERSION 4.1)

project(OOD)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SFML_INCLUDE_DIR "/opt/homebrew/include")
set(SFML_LIBRARY_DIR "/opt/homebrew/lib")
include_directories(${SFML_INCLUDE_DIR})
link_directories(${SFML_LIBRARY_DIR})
find_library(SFML_GRAPHICS_LIB sfml-graphics PATHS ${SFML_LIBRARY_DIR} REQUIRED)
find_library(SFML_WINDOW_LIB sfml-window PATHS ${SFML_LIBRARY_DIR} REQUIRED)
find_library(SFML_SYSTEM_LIB sfml-system PATHS ${SFML_LIBRARY_DIR} REQUIRED)
find_library(COCOA_LIB Cocoa REQUIRED)
find_library(IOKIT_LIB IOKit REQUIRED)
find_library(FOUNDATION_LIB Foundation REQUIRED)
find_library(OPENGL_LIB OpenGL REQUIRED)
find_package(GTest REQUIRED)
enable_testing()

find_program(CLANG_FORMAT clang-format)
file(GLOB_RECURSE ALL_SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)
add_custom_target(
        format-code
        COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
        COMMENT "Formatting all source code with clang-format"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Paint)
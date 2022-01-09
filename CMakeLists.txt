cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(
    "comp_graphics_cw2"
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/bin/res")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/res" DESTINATION "${CMAKE_BINARY_DIR}/bin")
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/my.scene")
    file(COPY "${CMAKE_CURRENT_LIST_DIR}/my.scene" DESTINATION "${CMAKE_BINARY_DIR}/bin")
endif()

add_subdirectory(lib)
add_subdirectory(src)
cmake_minimum_required(VERSION 3.0)

project("imgui")

file(GLOB SOURCES *.cpp *.h)
add_library(imgui STATIC ${SOURCES})
target_link_libraries(imgui glfw)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/)
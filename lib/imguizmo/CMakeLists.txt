cmake_minimum_required(VERSION 3.0)

project("imguizmo")

file(GLOB SOURCES *.cpp *.h)
add_library(imguizmo STATIC ${SOURCES})
target_link_libraries(imguizmo imgui)
target_include_directories(imguizmo PUBLIC ${CMAKE_CURRENT_LIST_DIR}/)
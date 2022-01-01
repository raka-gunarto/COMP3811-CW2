file(GLOB_RECURSE SOURCES *.cpp)

add_executable(prog ${SOURCES})
target_link_libraries(prog glad glfw glm imgui assimp imguizmo stb_image)
target_include_directories(prog PRIVATE ${CMAKE_CURRENT_LIST_DIR})
install(TARGETS prog RUNTIME DESTINATION bin)
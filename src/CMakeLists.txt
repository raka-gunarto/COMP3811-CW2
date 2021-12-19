file(GLOB_RECURSE SOURCES *.cpp)

add_executable(prog ${SOURCES})
target_link_libraries(prog glad glfw imgui)
install(TARGETS prog RUNTIME DESTINATION bin)
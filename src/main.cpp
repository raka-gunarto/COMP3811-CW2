#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <imgui/imgui.h>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Computer Graphics Coursework 2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "error: cannot create glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, WIDTH, HEIGHT);

    while(!glfwWindowShouldClose(window))
    {
        ImGui::Begin();
        ImGui::Text("Test");
        ImGui::End();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
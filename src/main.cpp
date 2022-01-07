#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <scene/scene.h>
#include <scene/object/components/component.h>
#include <scene/object/components/camera.h>
#include <scene/object/components/transform.h>
#include <scene/object/components/renderer/planeRenderer.h>
#include <scene/object/components/renderer/sphereRenderer.h>
#include <scene/object/scripts/camera/editCamera.h>

#include <ui/overview.h>
#include <ui/inspector.h>
#include <ui/objectHierarchy.h>
#include <ui/assets.h>

// stb implementation
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

std::shared_ptr<Scene> loadScene(GLFWwindow* w) {
    std::shared_ptr<Scene> s(new Scene(w));
    s->loadAssets();
    s->load();

    // add UI windows
    s->windowUIs.push_back(std::shared_ptr<Window>(new Overview(s)));
    s->windowUIs.push_back(std::shared_ptr<Window>(new ObjectHierarchy(s)));
    s->windowUIs.push_back(std::shared_ptr<Window>(new Inspector(s)));
    s->windowUIs.push_back(std::shared_ptr<Window>(new Assets(s)));

    return s;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Computer Graphics Coursework 2", glfwGetPrimaryMonitor(), NULL);
    if (window == NULL)
    {
        std::cout << "ERROR::GLFW::cannot create glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, mode->width, mode->height);
    glEnable(GL_DEPTH_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    std::shared_ptr<Scene> scene = loadScene(window);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(scene->backgroundColor.r, scene->backgroundColor.g, scene->backgroundColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->update();
        scene->render();
        scene->renderUI();

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
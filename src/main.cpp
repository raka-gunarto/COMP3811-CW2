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

#include <ui/overview.h>
#include <ui/inspector.h>
#include <ui/objectHierarchy.h>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

std::shared_ptr<Scene> constructDefaultScene(GLFWwindow* w) {
    std::shared_ptr<Scene> s(new Scene(w));

    // load our shaders
    std::shared_ptr<Shader> defaultShader(new Shader("default", "./res/shader/default/default.vert", "./res/shader/default/default.frag"));
    std::shared_ptr<Shader> activeShader(new Shader("active", "./res/shader/active/active.vert", "./res/shader/active/active.frag"));
    s->shaders.push_back(defaultShader);
    s->shaders.push_back(activeShader);

    // create ground plane
    std::shared_ptr<Object> groundPlane(new Object(s));
    groundPlane->setName("Ground Plane");
    std::shared_ptr<Transform> groundPlaneTransform(new Transform(groundPlane));
    groundPlaneTransform->scale.x = 1;
    groundPlaneTransform->scale.z = 1;
    groundPlane->components.push_back(std::shared_ptr<Component>(groundPlaneTransform));
    std::shared_ptr<PlaneRenderer> groundPlaneRenderer(new PlaneRenderer(groundPlane));
    groundPlaneRenderer->shader = s->shaders[0];
    groundPlane->components.push_back(std::shared_ptr<Component>(groundPlaneRenderer));
    s->objects.push_back(groundPlane);

    // create main camera
    std::shared_ptr<Object> mainCamera(new Object(s));
    mainCamera->setName("Main Camera");
    int width, height;
    glfwGetWindowSize(w, &width, &height);
    std::shared_ptr<Transform> mainCameraTransform(new Transform(mainCamera));
    mainCameraTransform->position.y = 1;
    mainCameraTransform->rotation.x = -45;
    mainCamera->components.push_back(std::shared_ptr<Component>(mainCameraTransform));
    mainCamera->components.push_back(std::shared_ptr<Component>(new Camera(mainCamera, width, height, 45.0f, 0.1f, 100.0f)));
    s->objects.push_back(mainCamera);
    s->activeCamera = std::dynamic_pointer_cast<Camera>(mainCamera->components[1]);

    // add UI windows
    s->windowUIs.push_back(std::shared_ptr<Window>(new Overview(s)));
    s->windowUIs.push_back(std::shared_ptr<Window>(new ObjectHierarchy(s)));
    s->windowUIs.push_back(std::shared_ptr<Window>(new Inspector(s)));

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

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    std::shared_ptr<Scene> scene = constructDefaultScene(window);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
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
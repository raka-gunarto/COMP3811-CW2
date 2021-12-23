#pragma once

#include <scene/object/object.h>
#include <scene/object/components/camera.h>
#include <ui/window.h>
#include <util/shader.h>

#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

class Object;
class Window;
class Shader;
class Camera;

class Scene : public std::enable_shared_from_this<Scene>
{
public:
    Scene(GLFWwindow* window) : window(window) {}
    void update();
    void render();
    void renderUI();

    std::shared_ptr<Camera> activeCamera;
    std::shared_ptr<Object> inspectedObject;

    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Window>> windowUIs;

    std::vector<std::shared_ptr<Shader>> shaders;
    std::vector<std::shared_ptr<Object>> blueprints;

    GLFWwindow* window;
private:
    void renderUI_Overview();
    void renderUI_ObjectTree();
    void renderUI_Inspector();

    bool vsync = true;
};
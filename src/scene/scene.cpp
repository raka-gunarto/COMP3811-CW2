#include "scene.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

//---BEGIN RANT---
// yes the uni computers have ancient versions of things 
// so I have to write this because writing OS-specific
// code is cringe and I can't use std::filesystem
// (and boost lib is 130 MB big)
//---END RANT---
#include <sstream>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __unix__
#include <ftw.h>
#endif
static std::vector<std::string> filesToProcess;
// very crude asset loading function
// fix later with c++17, running out of time for uni
// coursework submission
void processFiles(Scene* s)
{
    for (auto file : filesToProcess)
    {
        std::stringstream ss(file);
        std::string name, ext;

        // process all dir seps
        while(std::getline(ss, name, '/'));
        while(std::getline(ss, name, '\\'));
        ss = std::stringstream(name);

        // get name and extension
        name = "";
        while (std::getline(ss, ext, '.')) {
            name += ext;
            std::getline(ss, ext, '.');
        }
        if (ext == "jpg" || ext == "jpeg" || ext == "png" || ext == "bmp" || ext == "tga" || ext == "psd")
        { // process texture
            std::stringstream tss(name);
            std::string tk;
            std::vector<std::string> tks;
            while (std::getline(tss, tk, '_'))
                tks.push_back(tk);
            GLenum scale = GL_LINEAR, repeat = GL_REPEAT;
            if (tks.size() >= 2)
                if (tks[1] == "nearest")
                    scale = GL_NEAREST;
                else if (tks[1] == "linear")
                    scale = GL_LINEAR;
            if (tks.size() >= 3)
                if (tks[2] == "repeat")
                    repeat = GL_REPEAT;
                else if (tks[2] == "mirrored")
                    repeat = GL_MIRRORED_REPEAT;
                else if (tks[2] == "edge")
                    repeat = GL_CLAMP_TO_EDGE;
                else if (tks[2] == "border")
                    repeat = GL_CLAMP_TO_BORDER;

            s->textures.push_back(std::shared_ptr<Texture>(new Texture(Texture::loadFromFile(file.c_str()), GL_TEXTURE_2D, scale, repeat, glm::vec4(1.0f), name)));
        }
        else if (ext == "blend" || ext == "obj" || ext == "fbx")
        { // process model
            
        }
        else if (ext == "vert")
        { // process shader

        }
    }
}
void Scene::loadAssets(const char* path)
{
    // clear out the pending list
    std::vector<std::string>().swap(filesToProcess);
#ifdef _WIN32

#endif
#ifdef __unix
    nftw(path, [](const char* fpath, const struct stat* statb, int tflag, struct FTW* ftwb) {
        if (tflag == FTW_F)
            filesToProcess.push_back(std::string(fpath));
        return 0;
        }, 20, 0);
#endif
    processFiles(this);
}

void Scene::update() {
    // calculate deltaTime
    dTime = glfwGetTime() - dTime;
    for (auto obj : objects)
        obj->update(this->shared_from_this());

    // set the dtime to current time for next frame calc
    dTime = glfwGetTime();

    // reset scroll values
    scrollX = 0.0f;
    scrollY = 0.0f;
}

void Scene::render() {
    for (auto obj : objects)
        obj->render(this->shared_from_this());
}

void Scene::renderUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    for (auto window : windowUIs)
        window->render();

    ImGui::Render();
    // saves and restores opengl state (i love you imgui <3)
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
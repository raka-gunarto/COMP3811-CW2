#include "scene.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <scene/object/components/transform.h>
#include <scene/object/components/light.h>
#include <scene/object/components/renderer/meshRenderer.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "yaml-cpp/yaml.h"

#include <iostream>
#include <fstream>

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
std::shared_ptr<Mesh> processModelMesh(aiMesh* mesh, const aiScene* importedScene, std::string name)
{
    std::shared_ptr<Mesh> m(new Mesh());
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    // process vertices
    for (unsigned int vertIdx = 0; vertIdx < mesh->mNumVertices; ++vertIdx)
    {
        // positions
        vertices.push_back(mesh->mVertices[vertIdx].x);
        vertices.push_back(mesh->mVertices[vertIdx].y);
        vertices.push_back(mesh->mVertices[vertIdx].z);

        // normals
        if (!mesh->HasNormals())
        {
            std::cout << "ERROR::ASSETIMPROT::ASSIMP::" << name << "::mesh no normals" << std::endl;;
            return nullptr;
        }
        vertices.push_back(mesh->mNormals[vertIdx].x);
        vertices.push_back(mesh->mNormals[vertIdx].y);
        vertices.push_back(mesh->mNormals[vertIdx].z);

        // texcoords (unused for now, but the mesh construction expects them already)
        if (!mesh->mTextureCoords[0])
        {
            std::cout << "WARN::ASSETIMPROT::ASSIMP::" << name << "::mesh no tex coords::inserting placeholders" << std::endl;;
            vertices.push_back(0);
            vertices.push_back(0);
        }
        vertices.push_back(mesh->mTextureCoords[0][vertIdx].x);
        vertices.push_back(mesh->mTextureCoords[0][vertIdx].y);
    }
    // process indices
    for (unsigned int faceIdx = 0; faceIdx < mesh->mNumFaces; ++faceIdx)
        for (unsigned int indexIdx = 0; indexIdx < mesh->mFaces[faceIdx].mNumIndices; ++indexIdx)
            indices.push_back(mesh->mFaces[faceIdx].mIndices[indexIdx]);

    // process materials
    aiMaterial* mat = importedScene->mMaterials[mesh->mMaterialIndex];
    aiColor3D diffuse, specular;
    float shininess;
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    mat->Get(AI_MATKEY_SHININESS, shininess);

    m->diffuseColor.r = diffuse.r;
    m->diffuseColor.g = diffuse.g;
    m->diffuseColor.b = diffuse.b;
    m->specularColor.r = specular.r;
    m->specularColor.g = specular.g;
    m->specularColor.b = specular.b;
    m->shininess = shininess;
    // TODO: someday, import the textures as well but rn massive cba
    //       we'll use some nice low poly material prop only asset pack for now

    // construct mesh object
    m->constructMesh(vertices, indices);

    // name it to allow referencing
    m->name = name;
    return m;
}
bool processModelNode(aiNode* node, const aiScene* importedScene, std::string modelDir, std::shared_ptr<Object> blueprint)
{
    // process all meshes
    for (unsigned int meshIdx = 0; meshIdx < node->mNumMeshes; ++meshIdx)
    {
        // create new child for each mesh
        std::shared_ptr<Object> meshchild(new Object(blueprint->getScene()));
        meshchild->setName(blueprint->getName() + std::string("-m" + std::to_string(meshIdx)));

        // add mesh to child and scene
        std::shared_ptr<Mesh> mesh = processModelMesh(importedScene->mMeshes[node->mMeshes[meshIdx]], importedScene, meshchild->getName());
        if (mesh == nullptr) return false;
        meshchild->components.push_back(std::shared_ptr<Component>(new Transform(meshchild)));
        meshchild->components.push_back(std::shared_ptr<Component>(new MeshRenderer(meshchild, mesh)));
        meshchild->getComponent<MeshRenderer>()->mesh = mesh;
        blueprint->getScene()->meshes.push_back(mesh);

        // add child to parent
        meshchild->reparent(blueprint, true);
    }

    // process all node children
    for (unsigned int nodeIdx = 0; nodeIdx < node->mNumChildren; ++nodeIdx)
    {
        // create child
        std::shared_ptr<Object> child(new Object(blueprint->getScene()));
        child->reparent(blueprint, true);
        child->components.push_back(std::shared_ptr<Component>(new Transform(child)));
        child->setName(blueprint->getName() + std::string("-c" + std::to_string(nodeIdx)));

        // make child new blueprint parent in next processing
        if (!processModelNode(node->mChildren[nodeIdx], importedScene, modelDir, child))
            return false;
    }

    return true;
}
void processFiles(std::shared_ptr<Scene> s)
{
    for (auto file : filesToProcess)
    {
        std::stringstream ss(file);
        std::string name, ext;

        // process all dir seps
        while (std::getline(ss, name, '/'));
        while (std::getline(ss, name, '\\'));
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
            Assimp::Importer importer;
            const aiScene* importedScene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs);
            if (!importedScene || importedScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !importedScene->mRootNode)
            {
                std::cout << "WARN::ASSETIMPORT::ASSIMP::could not import " << file << std::endl;
                continue;
            }

            // create new blueprint (prefab)
            std::string modelDir = file.substr(0, file.find_last_of('/'));
            std::shared_ptr<Object> newBlueprint(new Object(s));
            newBlueprint->setName(std::string("importedmodel_" + name));
            newBlueprint->components.push_back(std::shared_ptr<Component>(new Transform(newBlueprint)));

            // populate blueprint with model meshes recursively and add to blueprints
            if (processModelNode(importedScene->mRootNode, importedScene, modelDir, newBlueprint))
                s->blueprints.push_back(newBlueprint);
            // if model loading fails, let the blueprint smart pointer die so everything
            // it owns will be released
        }
        else if (ext == "vert")
        { // process shader
            // find fragment shader
            std::string fragPath = "";
            for (auto fragFile : filesToProcess)
            {
                std::stringstream ss(fragFile);
                std::string fragname, fragext;

                // process all dir seps
                while (std::getline(ss, fragname, '/'));
                while (std::getline(ss, fragname, '\\'));
                ss = std::stringstream(fragname);

                // get name and extension
                fragname = "";
                while (std::getline(ss, fragext, '.')) {
                    fragname += fragext;
                    std::getline(ss, fragext, '.');
                }

                if (fragname == name && fragext == "frag")
                {
                    fragPath = fragFile;
                    break;
                }
            }
            if (fragPath == "")
            {
                std::cout << "ERROR::ASSETIMPORT::SHADER::could not find fragment shader for " << file << std::endl;
                continue;
            }

            std::shared_ptr<Shader> shader(new Shader(name, file.c_str(), fragPath.c_str()));
            s->shaders.push_back(shader);
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
    processFiles(this->shared_from_this());
}

void findLightsRecursive(Scene* s, std::shared_ptr<Object> o)
{
    auto light = o->getComponent<Light>();
    if (light && light->type == Light::Type::POINT && s->lights.size() < Scene::MAX_LIGHTS)
        s->lights.push_back(light);

    if (light && light->type == Light::Type::DIRECTIONAL)
        s->dirLight = light;

    for (auto obj : o->children)
        findLightsRecursive(s, obj);
}

void shaderUniforms(Scene* s)
{
    // reset light uniforms
    for (auto shader : s->shaders)
    {
        for (int i = 0; i < s->lights.size(); ++i)
        {
            glUniform3f(glGetUniformLocation(shader->id,
                std::string("lights[" + std::to_string(i) + "].pos").c_str()),
                0,
                0,
                0
            );
            glUniform3f(glGetUniformLocation(shader->id,
                std::string("lights[" + std::to_string(i) + "].color").c_str()),
                0,
                0,
                0
            );
            glUniform1f(glGetUniformLocation(shader->id,
                std::string("lights[" + std::to_string(i) + "].linAttenuate").c_str()),
                0
            );
            glUniform1f(glGetUniformLocation(shader->id,
                std::string("lights[" + std::to_string(i) + "].quadAttenuate").c_str()),
                0
            );
        }
        glUniform3f(glGetUniformLocation(shader->id, "sun.dir"),
            0,
            0,
            0
        );
        glUniform3f(glGetUniformLocation(shader->id, "sun.color"),
            0,
            0,
            0
        );
    }

    // set uniforms
    for (auto shader : s->shaders)
    {
        shader->activate();

        // scene ambient parameters
        glUniform3f(glGetUniformLocation(shader->id, "ambientColor"),
            s->ambientColor.r,
            s->ambientColor.g,
            s->ambientColor.b
        );
        glUniform1f(glGetUniformLocation(shader->id, "ambientIntensity"), s->ambientIntensity);

        // specular lighting, camera position
        glUniform3f(glGetUniformLocation(shader->id, "cameraPos"),
            s->activeCamera->transform->position.x,
            s->activeCamera->transform->position.y,
            s->activeCamera->transform->position.z
        );

        // background color of scene
        glUniform3f(glGetUniformLocation(shader->id, "backgroundColor"),
            s->backgroundColor.r,
            s->backgroundColor.g,
            s->backgroundColor.b
        );

        // camera matrix (view + projection) and fog params
        glUniformMatrix4fv(glGetUniformLocation(shader->id, "cameraMat"), 1, GL_FALSE, glm::value_ptr(s->activeCamera->getMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(shader->id, "view"), 1, GL_FALSE, glm::value_ptr(s->activeCamera->getView()));
        glUniform1f(glGetUniformLocation(shader->id, "farPlane"), s->activeCamera->far);
        glUniform1f(glGetUniformLocation(shader->id, "fogOffset"), s->activeCamera->fogOffset);

        // lights
        for (int i = 0; i < s->lights.size(); ++i)
        {
            glUniform3f(glGetUniformLocation(shader->id,
                std::string("lights[" + std::to_string(i) + "].pos").c_str()),
                s->lights[i]->transform->worldPos().x,
                s->lights[i]->transform->worldPos().y,
                s->lights[i]->transform->worldPos().z
            );
            glUniform3f(glGetUniformLocation(shader->id,
                std::string("lights[" + std::to_string(i) + "].color").c_str()),
                s->lights[i]->color.r,
                s->lights[i]->color.g,
                s->lights[i]->color.b
            );
            glUniform1f(glGetUniformLocation(shader->id,
                std::string("lights[" + std::to_string(i) + "].linAttenuate").c_str()),
                s->lights[i]->linearAttenuation
            );
            glUniform1f(glGetUniformLocation(shader->id,
                std::string("lights[" + std::to_string(i) + "].quadAttenuate").c_str()),
                s->lights[i]->quadAttenuation
            );
        }
        // extract rotation from directional light transform
        if (s->dirLight)
        {
            glm::vec3 sunDirection = glm::rotate(
                glm::quat(s->dirLight->transform->modelMatrix()),
                glm::vec3(0, 1.0f, 0)
            );
            glUniform3f(glGetUniformLocation(shader->id, "sun.dir"),
                sunDirection.x,
                sunDirection.y,
                sunDirection.z
            );
            glUniform3f(glGetUniformLocation(shader->id, "sun.color"),
                s->dirLight->color.r,
                s->dirLight->color.g,
                s->dirLight->color.b
            );
        }
    }
}

void Scene::update() {
    // find all lights in scene
    lights.clear();
    for (auto obj : objects)
        findLightsRecursive(this, obj);

    // set all shader uniforms that can be set
    shaderUniforms(this);

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

// scene saving
void Scene::save(std::string filename)
{
    YAML::Emitter serialiser;

    // objects as sequence of maps
    serialiser << YAML::BeginSeq;
    for (auto obj : objects)
        serialiser << *obj; // expect map
    serialiser << YAML::EndSeq;

    std::ofstream outfile(filename);
    if (outfile)
    {
        outfile << serialiser.c_str();
        outfile.close();
    }
    else
        std::cout << "ERROR::SCENE::SERIALISER::could not open file" << std::endl;
}

// scene loading
void Scene::load(std::string filename)
{
    YAML::Node sceneNode = YAML::LoadFile(filename);
    if (!sceneNode || !sceneNode.IsSequence())
        std::cout << "ERROR::SCENE::DESERIALISER::can't read file" << std::endl;

    for (auto it = sceneNode.begin(); it != sceneNode.end(); ++it)
        objects.push_back(Object::deserialise(*it, this->shared_from_this()));

    for (auto obj : objects)
    {
        auto cam = obj->getComponent<Camera>();
        if (cam)
        {
            activeCamera = cam;
            break;
        }
    }
}
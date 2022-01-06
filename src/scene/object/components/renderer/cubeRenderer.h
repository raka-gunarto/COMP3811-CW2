#pragma once

#include <scene/object/components/renderer/renderer.h>
#include <util/vbo.h>
#include <util/vao.h>
#include <util/ebo.h>

#include <glm/glm.hpp>

#include <memory>

class Object;
class Texture;
class VAO;
class VBO;
class EBO;

class CubeRenderer : public Renderer {
public:
    CubeRenderer(std::shared_ptr<Object> obj);
    CubeRenderer(const CubeRenderer& other) = delete;
    CubeRenderer(const CubeRenderer& other, std::shared_ptr<Object> newObj)
        : CubeRenderer(newObj)
    {
        diffuseColor = other.diffuseColor;
        specularColor = other.specularColor;
        shininess = other.shininess;
        diffuseTex = other.diffuseTex;
        specularTex = other.specularTex;
    }
    std::shared_ptr<Component> clone(std::shared_ptr<Object> newObj) {
        return std::shared_ptr<Component>(new CubeRenderer(*this, newObj));
    }

    void render(std::shared_ptr<Scene> s) override;
    void renderInspector() override;

    static void initVertexData();

    enum Mode
    {
        MATERIAL,
        TEX_MAP
    };
    CubeRenderer::Mode mode;

    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess = 1.0f;
    std::shared_ptr<Texture> diffuseTex;
    std::shared_ptr<Texture> specularTex;


protected:
    static bool initialised;
    // every renderer uses the same cube, make them static
    static std::shared_ptr<VBO> cubeVBO;
    static std::shared_ptr<VAO> cubeVAO;
    static std::shared_ptr<EBO> cubeEBO;
};
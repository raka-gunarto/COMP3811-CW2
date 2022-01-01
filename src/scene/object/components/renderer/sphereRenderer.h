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

class SphereRenderer : public Renderer {
public:
    SphereRenderer(std::shared_ptr<Object> obj);
    SphereRenderer(const SphereRenderer& other) = delete;
    SphereRenderer(const SphereRenderer& other, std::shared_ptr<Object> newObj)
        : SphereRenderer(newObj)
    {
        diffuseColor = other.diffuseColor;
        specularColor = other.specularColor;
        shininess = other.shininess;
        diffuseTex = other.diffuseTex;
        specularTex = other.specularTex;
    }
    std::shared_ptr<Component> clone(std::shared_ptr<Object> newObj) {
        return std::shared_ptr<Component>(new SphereRenderer(*this, newObj));
    }

    void render(std::shared_ptr<Scene> s) override;
    void renderInspector() override;

    static void initVertexData();

    enum Mode
    {
        MATERIAL,
        TEX_MAP
    };
    SphereRenderer::Mode mode;

    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
    std::shared_ptr<Texture> diffuseTex;
    std::shared_ptr<Texture> specularTex;

    static const int STACKS = 50;
    static const int SECTORS = 50;

protected:
    static bool initialised;
    // every renderer uses the same sphere, make them static
    static std::shared_ptr<VBO> sphereVBO;
    static std::shared_ptr<VAO> sphereVAO;
    static std::shared_ptr<EBO> sphereEBO;
};
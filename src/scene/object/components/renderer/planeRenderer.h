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

class PlaneRenderer : public Renderer {
public:
    PlaneRenderer(std::shared_ptr<Object> obj);

    void render(std::shared_ptr<Scene> s) override;
    void renderInspector() override;

    static void initVertexData();

    enum Mode
    {
        MATERIAL,
        TEX_MAP
    };
    PlaneRenderer::Mode mode;

    glm::vec3 diffuseColor;
    float specularIntensity;
    std::shared_ptr<Texture> diffuseTex;
    std::shared_ptr<Texture> specularTex;


protected:
    static bool initialised;
    // every renderer uses the same plane, make them static
    static std::shared_ptr<VBO> planeVBO;
    static std::shared_ptr<VAO> planeVAO;
    static std::shared_ptr<EBO> planeEBO;
};
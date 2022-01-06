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
    PlaneRenderer(const PlaneRenderer& other) = delete;
    PlaneRenderer(const PlaneRenderer& other, std::shared_ptr<Object> newObj)
        : PlaneRenderer(newObj)
    {
        diffuseColor = other.diffuseColor;
        specularColor = other.specularColor;
        shininess = other.shininess;
        diffuseTex = other.diffuseTex;
        specularTex = other.specularTex;
    }
    std::shared_ptr<Component> clone(std::shared_ptr<Object> newObj) {
        return std::shared_ptr<Component>(new PlaneRenderer(*this, newObj));
    }

    void render(std::shared_ptr<Scene> s) override;
    void renderInspector() override;
    YAML::Emitter& serialise(YAML::Emitter& emitter) override
    {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name";
        emitter << YAML::Value << "PlaneRenderer";
        emitter << YAML::Key << "mode" << YAML::Value << mode;
        emitter << YAML::Key << "diffuseColor" << YAML::Value << YAML::Flow << YAML::BeginSeq
            << diffuseColor.r << diffuseColor.g << diffuseColor.b
            << YAML::EndSeq;
        emitter << YAML::Key << "specularColor" << YAML::Value << YAML::Flow << YAML::BeginSeq
            << specularColor.r << specularColor.g << specularColor.b
            << YAML::EndSeq;
        emitter << YAML::Key << "shininess" << YAML::Value << shininess;
        if (diffuseTex)
            emitter << YAML::Key << "diffuseTex" << YAML::Value << diffuseTex->name;
        if (specularTex)
            emitter << YAML::Key << "specularTex" << YAML::Value << specularTex->name;
        emitter << YAML::EndMap;
        return emitter;
    }

    static void initVertexData();

    enum Mode
    {
        MATERIAL,
        TEX_MAP
    };
    PlaneRenderer::Mode mode;

    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess = 1.0f;
    std::shared_ptr<Texture> diffuseTex = nullptr;
    std::shared_ptr<Texture> specularTex = nullptr;


protected:
    static bool initialised;
    // every renderer uses the same plane, make them static
    static std::shared_ptr<VBO> planeVBO;
    static std::shared_ptr<VAO> planeVAO;
    static std::shared_ptr<EBO> planeEBO;
};
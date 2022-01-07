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
    YAML::Emitter& serialise(YAML::Emitter& emitter) override
    {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name";
        emitter << YAML::Value << "SphereRenderer";
        if (shader)
            emitter << YAML::Key << "shader" << YAML::Value << shader->name;
        else
            emitter << YAML::Key << "shader" << YAML::Value << "default";
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
    void _deserialise(const YAML::Node& componentNode) override
    {
        // do the reverse of serialise duh
        // find shader
        if (componentNode["shader"])
        {
            std::string shaderName = componentNode["shader"].as<std::string>();
            for (auto s : object->getScene()->shaders)
                if (s->name == shaderName)
                    shader = s;
        }
        if (!shader) // should probably output a warning but cba rn
            shader = object->getScene()->shaders[0];

        // find textures
        if (componentNode["diffuseTex"])
        {
            std::string texName = componentNode["diffuseTex"].as<std::string>();
            for (auto t : object->getScene()->textures)
                if (t->name == texName)
                    diffuseTex = t;
        }
        if (componentNode["specularTex"])
        {
            std::string texName = componentNode["specularTex"].as<std::string>();
            for (auto t : object->getScene()->textures)
                if (t->name == texName)
                    specularTex = t;
        }

        // set the rest (im giving up on type checking now)
        mode = (Mode) componentNode["mode"].as<int>();
        diffuseColor = glm::vec3(
            componentNode["diffuseColor"][0].as<float>(),
            componentNode["diffuseColor"][1].as<float>(),
            componentNode["diffuseColor"][2].as<float>()
        );
        specularColor = glm::vec3(
            componentNode["specularColor"][0].as<float>(),
            componentNode["specularColor"][1].as<float>(),
            componentNode["specularColor"][2].as<float>()
        );
        shininess = componentNode["shininess"].as<float>();
    }

    static void initVertexData();

    enum Mode
    {
        MATERIAL,
        TEX_MAP
    };
    SphereRenderer::Mode mode;

    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess = 1.0f;
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
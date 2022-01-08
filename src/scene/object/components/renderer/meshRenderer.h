#pragma once

#include <scene/object/components/renderer/renderer.h>
#include <util/mesh.h>

#include <stdexcept>

class MeshRenderer : public Renderer {
public:
    MeshRenderer(std::shared_ptr<Object> obj, std::shared_ptr<Mesh> mesh = nullptr)
        : Renderer(obj), mesh(mesh)
    {
        name = "MeshRenderer";
    }
    MeshRenderer(const MeshRenderer& other, std::shared_ptr<Object> newObj)
        : MeshRenderer(newObj, other.mesh) {}

    void render(std::shared_ptr<Scene> s, std::shared_ptr<Shader> shaderOverride = nullptr) override;
    void renderInspector() override;
    YAML::Emitter& serialise(YAML::Emitter& emitter) override
    {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name";
        emitter << YAML::Value << "MeshRenderer";
        if (shader)
            emitter << YAML::Key << "shader" << YAML::Value << shader->name;
        else
            emitter << YAML::Key << "shader" << YAML::Value << "default";
        if (mesh)
            emitter << YAML::Key << "mesh" << YAML::Value << mesh->name;

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

        // find mesh
        if (componentNode["mesh"])
        {
            std::string meshName = componentNode["mesh"].as<std::string>();
            for (auto m : object->getScene()->meshes)
                if (m->name == meshName)
                    mesh = m;
        }
    }
    std::shared_ptr<Component> clone(std::shared_ptr<Object> newObj) {
        return std::shared_ptr<Component>(new MeshRenderer(*this, newObj));
    }

    std::shared_ptr<Mesh> mesh;
};
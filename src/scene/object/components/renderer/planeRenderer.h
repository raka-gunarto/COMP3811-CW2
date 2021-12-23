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

    enum Mode
    {
        FLAT_COLOR,
        TEXTURE
    };
    PlaneRenderer::Mode mode;

    glm::vec3 color;
    std::shared_ptr<Texture> tex;

private:
    std::shared_ptr<VBO> planeVBO;
    std::shared_ptr<VAO> planeVAO;
    std::shared_ptr<EBO> planeEBO;
};
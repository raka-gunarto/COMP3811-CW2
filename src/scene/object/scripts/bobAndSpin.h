#pragma once

#include <scene/object/scripts/script.h>

#include <string>
#include <memory>

class Object;
class Scene;
class Transform;

class BobAndSpin : public Script {
public:
    BobAndSpin(std::shared_ptr<Object> obj) : Script("BobAndSpin", obj) {
        start();
    }
    BobAndSpin(std::shared_ptr<Object> obj, float bobSpeed, float bobOffset, float bobSize, float spinSpeed)
        : BobAndSpin(obj)
    {
        this->bobSpeed = bobSpeed;
        this->bobOffset = bobOffset;
        this->bobSize = bobSize;
        this->spinSpeed = spinSpeed;
    }

    BobAndSpin(const BobAndSpin& other) = delete;
    BobAndSpin(const BobAndSpin& other, std::shared_ptr<Object> newObj)
        : BobAndSpin(newObj, other.bobSpeed, other.bobOffset, other.bobSize, other.spinSpeed) {}
    void start() override;
    void update(std::shared_ptr<Scene> s) override;
    void renderInspector() override;
    YAML::Emitter& serialise(YAML::Emitter& emitter) override
    {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name";
        emitter << YAML::Value << "BobAndSpin";
        emitter << YAML::Key << "bobSpeed" << YAML::Value << bobSpeed;
        emitter << YAML::Key << "bobOffset" << YAML::Value << bobOffset;
        emitter << YAML::Key << "bobSize" << YAML::Value << bobSize;
        emitter << YAML::Key << "spinSpeed" << YAML::Value << spinSpeed;
        emitter << YAML::EndMap;
        return emitter;
    }
    void _deserialise(const YAML::Node& componentNode) override
    {
        // do the reverse of serialise duh
        bobSpeed = componentNode["bobSpeed"].as<double>();
        bobOffset = componentNode["bobOffset"].as<double>();
        bobSize = componentNode["bobSize"].as<double>();
        spinSpeed = componentNode["spinSpeed"].as<double>();
    }

    std::shared_ptr<Script> clone(std::shared_ptr<Object> newObj) {
        return std::shared_ptr<Script>(new BobAndSpin(*this, newObj));
    }

    float bobSpeed = 1.0f;
    float bobOffset = 3.0f;
    float bobSize = 2.0f;
    float spinSpeed = 1.0f;
private:
    std::shared_ptr<Transform> t;
};
#pragma once

#include <scene/object/scripts/script.h>

#include <string>
#include <memory>

class Object;
class Scene;
class Transform;

class SunMoonCycle : public Script {
public:
    SunMoonCycle(std::shared_ptr<Object> obj) : Script("SunMoonCycle", obj) {
        start();
    }
    SunMoonCycle(std::shared_ptr<Object> obj, float time, float timeScale)
        : SunMoonCycle(obj)
    {
        this->time = time;
        this->timeScale = timeScale;
    }

    SunMoonCycle(const SunMoonCycle& other) = delete;
    SunMoonCycle(const SunMoonCycle& other, std::shared_ptr<Object> newObj)
        : SunMoonCycle(newObj, other.time, other.timeScale) {}
    void start() override;
    void update(std::shared_ptr<Scene> s) override;
    void renderInspector() override;
    YAML::Emitter& serialise(YAML::Emitter& emitter) override
    {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name";
        emitter << YAML::Value << "SunMoonCycle";
        emitter << YAML::Key << "time" << YAML::Value << time;
        emitter << YAML::Key << "timeScale" << YAML::Value << timeScale;
        emitter << YAML::EndMap;
        return emitter;
    }
    void _deserialise(const YAML::Node& componentNode) override
    {
        // do the reverse of serialise duh
        time = componentNode["time"].as<double>();
        timeScale = componentNode["timeScale"].as<double>();
    }

    std::shared_ptr<Script> clone(std::shared_ptr<Object> newObj) {
        return std::shared_ptr<Script>(new SunMoonCycle(*this, newObj));
    }

    double time = 0.0f;
    double timeScale = 60.0f;
private:
    std::shared_ptr<Transform> t;
};
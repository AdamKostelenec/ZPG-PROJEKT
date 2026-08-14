#pragma once
#include <glm/glm.hpp>
#include <string>

class ShaderProgram;

enum class LightType {
    Directional = 0,
    Point = 1,
    Spot = 2,
    Ambient = 3
};

struct Attenuation {
    float constant = 1.0f;
    float linear = 0.0f;
    float quadratic = 0.0f;
};

class Light {
public:
    LightType type = LightType::Point;
    glm::vec3 position{ 0.0f, 3.0f, 0.0f };
    glm::vec3 direction{ 0.0f, -1.0f, 0.0f };
    glm::vec3 color{ 1.0f, 1.0f, 1.0f };
    float     intensity = 1.0f;
    float     cutoff = glm::cos(glm::radians(15.0f));
    Attenuation atten{ 1.0f, 0.09f, 0.032f };

    static Light makeDirectional(const glm::vec3& dir, const glm::vec3& col, float intens = 1.0f);
    static Light makePoint(const glm::vec3& pos, const glm::vec3& col, float intens = 1.0f,
        const Attenuation& a = { 1.0f, 0.09f, 0.032f });
    static Light makeSpot(const glm::vec3& pos, const glm::vec3& dir,
        const glm::vec3& col, float intens, float cutoffRadians,
        const Attenuation& a = { 1.0f, 0.09f, 0.032f });
    static Light makeAmbient(const glm::vec3& col, float intens = 1.0f);

    void uploadOne(const ShaderProgram& prog, const std::string& baseName, int index) const;
};
void uploadLights(const ShaderProgram& prog, const std::vector<Light>& lights);

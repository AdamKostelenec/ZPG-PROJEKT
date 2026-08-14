#include "Light.h"
#include "ShaderProgram.h"
#include <glm/gtc/matrix_transform.hpp>

Light Light::makeDirectional(const glm::vec3& dir, const glm::vec3& col, float intens) {
    Light l;
    l.type = LightType::Directional;
    l.direction = glm::normalize(dir);
    l.color = col;
    l.intensity = intens;
    return l;
}

Light Light::makePoint(const glm::vec3& pos, const glm::vec3& col, float intens,
    const Attenuation& a) {
    Light l;
    l.type = LightType::Point;
    l.position = pos;
    l.color = col;
    l.intensity = intens;
    l.atten = a;
    return l;
}

Light Light::makeSpot(const glm::vec3& pos, const glm::vec3& dir,
    const glm::vec3& col, float intens, float cutoffRadians,
    const Attenuation& a) {
    Light l;
    l.type = LightType::Spot;
    l.position = pos;
    l.direction = glm::normalize(dir);
    l.color = col;
    l.intensity = intens;
    l.cutoff = glm::cos(cutoffRadians);
    l.atten = a;
    return l;
}

Light Light::makeAmbient(const glm::vec3& col, float intens) {
    Light l;
    l.type = LightType::Ambient;
    l.color = col;
    l.intensity = intens;
    return l;
}

void Light::uploadOne(const ShaderProgram& prog, const std::string& baseName, int index) const {
    std::string prefix = baseName + "[" + std::to_string(index) + "].";
    prog.setUniform((prefix + "position").c_str(), position);
    prog.setUniform((prefix + "direction").c_str(), direction);
    prog.setUniform((prefix + "color").c_str(), color);
    prog.setUniform((prefix + "intensity").c_str(), intensity);
    prog.setUniform((prefix + "cutoff").c_str(), cutoff);
    prog.setUniform((prefix + "atten").c_str(),
        glm::vec3(atten.constant, atten.linear, atten.quadratic));
    prog.setUniform((prefix + "type").c_str(), static_cast<int>(type));
}


void uploadLights(const ShaderProgram& prog, const std::vector<Light>& lights)
{
    prog.use();
    int n = static_cast<int>(lights.size());
    if (n > 16) n = 16; 

    prog.setUniform("uNumLights", n);
    for (int i = 0; i < n; ++i) {
        lights[i].uploadOne(prog, "uLight", i);
    }
}

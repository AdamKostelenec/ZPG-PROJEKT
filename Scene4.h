#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "ShaderProgram.h"

// Sluneční soustava – scéna 4
class Scene4 {
public:
    Scene4() = default;

    void init(Scene& scene,
        const Mesh& meshSphere,
        ShaderProgram* phong);

    void update(float dt);

    const std::vector<Light>& lights() const { return lightsScene4_; }

private:
    Scene* scene_ = nullptr;
    ShaderProgram* phongShader_ = nullptr;
    const Mesh* meshSphere_ = nullptr;

    std::vector<Light> lightsScene4_;

    SceneNode* sunNode_ = nullptr;

    SceneNode* zemOrbit_ = nullptr;
    SceneNode* earthSpin_ = nullptr;

    SceneNode* moonOrbit_ = nullptr;
    SceneNode* moonSpin_ = nullptr;

    SceneNode* merOrbit_ = nullptr;
    SceneNode* merSpin_ = nullptr;

    SceneNode* venOrbit_ = nullptr;
    SceneNode* venSpin_ = nullptr;

    SceneNode* marOrbit_ = nullptr;
    SceneNode* marSpin_ = nullptr;

    SceneNode* jupOrbit_ = nullptr;
    SceneNode* jupSpin_ = nullptr;

    SceneNode* satOrbit_ = nullptr;
    SceneNode* satSpin_ = nullptr;

    SceneNode* uraOrbit_ = nullptr;
    SceneNode* uraSpin_ = nullptr;

    SceneNode* nepOrbit_ = nullptr;
    SceneNode* nepSpin_ = nullptr;

    float time_ = 0.0f;  
};

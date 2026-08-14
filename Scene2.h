#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "ShaderProgram.h"
#include "Draweble.h"

class Scene2 {
public:
    Scene2() = default;

    void init(Scene& scene,
        const Mesh& meshSphere,
        ShaderProgram* phong);

    void update(float dt);

    const std::vector<Light>& lights() const { return lightsScene2_; }

private:
    Scene* scene_ = nullptr;
    ShaderProgram* phongShader_ = nullptr;
    const Mesh* meshSphere_ = nullptr;

    SceneNode* zemOrbit_ = nullptr;   
    SceneNode* earthSpin_ = nullptr; 
	SceneNode* rotate_ = nullptr;
    SceneNode* ballrotate_ = nullptr;

    float time_ = 0.0f;
    std::vector<Light> lightsScene2_;
};

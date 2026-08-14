// Scene6.h
#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "ShaderProgram.h"
#include "Draweble.h"


class Scene6 {
public:
    Scene6() = default;

    void init(Scene& scene,
        const Mesh& meshSphere,
        ShaderProgram* phong);

    void update(float /*dt*/) {}

    const std::vector<Light>& lights() const { return lightsScene6_; }

private:
    Scene* scene_ = nullptr;
    ShaderProgram* phongShader_ = nullptr;
    const Mesh* meshSphere_ = nullptr;

    std::vector<Light> lightsScene6_;
};

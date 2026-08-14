// Scene5.h
#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "ShaderProgram.h"

class Scene5 {
public:
    Scene5() = default;


    void init(Scene& scene,
        const Mesh& meshGround,
        const Mesh& meshFormule,
        ShaderProgram* phong);


    void update(float dt, bool bezierMode);

    const std::vector<Light>& lights() const { return lightsScene5_; }

    void addBezierPoint(const glm::vec3& worldPos);

    void resetBezier();

private:
    void updateBezier_(float dt);

    Scene* scene_ = nullptr;
    ShaderProgram* phongShader_ = nullptr;
    const Mesh* meshGround_ = nullptr;
    const Mesh* meshFormule_ = nullptr;


    std::vector<Light>      lightsScene5_;

    SceneNode* bezierNode_ = nullptr;   
    std::vector<glm::vec3>  pathPoints_;               // body dráhy
    int                     currentCurve_ = 0;
    float                   currentT_ = 0.0f;
};

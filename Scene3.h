// Scene3.h
#pragma once
#include <vector>
#include <string>
#include <random>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "ShaderProgram.h"
#include "Draweble.h"
#include "Transform.h"
#include "TextureLoader.h"

class Scene3 {
public:
    Scene3() = default;

    void init(Scene& scene,
        const Mesh& meshSphere,
        const Mesh& meshGround,
        const Mesh& meshTree,
        const Mesh& meshBush,
        const Mesh& meshShrek,
        const Mesh& meshFiona,
        const Mesh& meshToiled,
        const Mesh& meshMushroom,
        const std::vector<std::string>& shrekTexPaths,
        const std::vector<std::string>& fionaTexPaths,
        const std::vector<std::string>& toiledTexPaths,
        const std::vector<std::string>& mushroomTexPaths,
        ShaderProgram* phong);

    void update(float dt);

    const std::vector<Light>& lights() const { return lightsScene3_; }


    void onPick(SceneNode*& selectedNode,
        const glm::vec3& lastClickWorldPos,
        const glm::vec3& cameraPos);

private:
    void updateFireflies_(float dt);

    Scene* scene_ = nullptr;
    ShaderProgram* phongShader_ = nullptr;

    const Mesh* meshSphere_ = nullptr;
    const Mesh* meshGround_ = nullptr;
    const Mesh* meshTree_ = nullptr;
    const Mesh* meshBush_ = nullptr;
    const Mesh* meshShrek_ = nullptr;
    const Mesh* meshFiona_ = nullptr;
    const Mesh* meshToiled_ = nullptr;
    const Mesh* meshMushroom_ = nullptr;

    const std::vector<std::string>* shrekTexPaths_ = nullptr;
    const std::vector<std::string>* fionaTexPaths_ = nullptr;
    const std::vector<std::string>* toiledTexPaths_ = nullptr;
    const std::vector<std::string>* mushroomTexPaths_ = nullptr;

    // stav scény
    std::vector<Light>        lightsScene3_;
    std::vector<glm::vec3>    fireflyPos_;
    std::vector<glm::vec3>    fireflyVel_;
    std::vector<SceneNode*>   fireflyNodes_;

    std::vector<SceneNode*>   mushroomNodes_;
    int mushroomsCollected_ = 0;

    std::mt19937 rngFireflies_{ 1337 };
};

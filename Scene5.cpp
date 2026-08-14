// Scene5.cpp
#include "Scene5.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "Draweble.h"
#include "Transform.h"

void Scene5::init(Scene& scene,
    const Mesh& meshGround,
    const Mesh& meshFormule,
    ShaderProgram* phong)
{
    scene_ = &scene;
    meshGround_ = &meshGround;
    meshFormule_ = &meshFormule;
    phongShader_ = phong;

    auto root = SceneNode::makeGroup();


    lightsScene5_.clear();
    lightsScene5_ = {
        Light::makePoint({ 0,10,0 }, { 1,1,1 }, 1.2f, { 1,0,0 }),
        Light::makeAmbient({1.0f, 1.0f, 1.0f}, 0.8f)
    };


    SceneNode* road = scene_->addDrawableObject(
        root.get(), *meshGround_,
        { 0,0,0 }, { 1,1,1 },
        { 0.38f, 0.38f, 0.40f, 1.0f },
        phongShader_
    );
    if (road && road->drawable()) {
        road->drawable()->setShadeModel(0); 
    }

    Material formuleMat{ 0.15f,0.80f,0.60f,48.0f };
    SceneNode* formuleNode = scene_->addDrawableObject(
        root.get(), *meshFormule_,
        { 0.0f, 0.0f, 0.0f },
        { 0.1f, 0.1f, 0.1f },
        { 1,1,1,1 },
        phongShader_,formuleMat
    );
    bezierNode_ = formuleNode;


    pathPoints_.clear();

    pathPoints_.push_back({ 16.0f, 0.0f, -10.0f });
    pathPoints_.push_back({ 18.0f, 0.0f,  -6.0f });
    pathPoints_.push_back({ 18.0f, 1.0f,  -2.0f });

    pathPoints_.push_back({ 17.0f, 2.0f,   2.5f });
    pathPoints_.push_back({ 15.0f, 3.0f,   6.5f });
    pathPoints_.push_back({ 11.5f, 2.0f,   9.5f });
    pathPoints_.push_back({ 7.0f, 1.0f,  11.5f });

    pathPoints_.push_back({ 1.5f, 0.0f,  12.5f });
    pathPoints_.push_back({ -3.0f, 0.0f,  12.0f });
    pathPoints_.push_back({ -7.5f, 0.0f,  10.5f });
    pathPoints_.push_back({ -11.0f,0.0f,   8.0f });

    pathPoints_.push_back({ -13.5f,0.0f,   4.5f });
    pathPoints_.push_back({ -14.5f,0.0f,   0.5f });
    pathPoints_.push_back({ -14.0f,0.0f,  -3.5f });

    pathPoints_.push_back({ -12.5f,0.0f,  -7.5f });
    pathPoints_.push_back({ -9.5f,0.0f, -10.5f });
    pathPoints_.push_back({ -5.5f,0.0f, -12.5f });

    pathPoints_.push_back({ -1.5f,0.0f, -13.0f });
    pathPoints_.push_back({ 2.5f,0.0f, -12.5f });
    pathPoints_.push_back({ 5.5f,0.0f, -11.0f });
    pathPoints_.push_back({ 7.5f,0.0f,  -8.5f });
    pathPoints_.push_back({ 6.0f,0.0f,  -6.0f });
    pathPoints_.push_back({ 3.5f,0.0f,  -4.5f });
    pathPoints_.push_back({ 0.5f,0.0f,  -4.0f });

    pathPoints_.push_back({ 4.0f,0.0f,  -2.0f });
    pathPoints_.push_back({ 8.0f,0.0f,  -1.0f });
    pathPoints_.push_back({ 11.5f,0.0f,  -2.0f });
    pathPoints_.push_back({ 14.0f,0.0f,  -4.5f });

    pathPoints_.push_back({ 15.5f,0.0f,  -7.0f });

    currentCurve_ = 0;
    currentT_ = 0.0f;

    scene_->addRoot(std::move(root));
}

void Scene5::update(float dt, bool bezierMode)
{
    if (bezierMode)
        return;

    updateBezier_(dt);
}

void Scene5::addBezierPoint(const glm::vec3& worldPos)
{
    pathPoints_.push_back(worldPos);
}

void Scene5::resetBezier()
{
    pathPoints_.clear();
    currentCurve_ = 0;
    currentT_ = 0.0f;
}


void Scene5::updateBezier_(float dt)
{
    if (!bezierNode_ || pathPoints_.size() < 4)
        return;

    const int n = static_cast<int>(pathPoints_.size());
    if (n < 4) return;

    const int   segCount = n;
    const float speed = 2.0f;

    currentT_ += speed * dt;

    while (currentT_ > 1.0f) {
        currentT_ -= 1.0f;
        ++currentCurve_;
        if (currentCurve_ >= segCount) {
            currentCurve_ = 0;
        }
    }

    int i1 = currentCurve_;
    int i2 = (i1 + 1) % n;
    int i0 = (i1 - 1 + n) % n;
    int i3 = (i1 + 2) % n;

    const glm::vec3& W0 = pathPoints_[i0];
    const glm::vec3& W1 = pathPoints_[i1];
    const glm::vec3& W2 = pathPoints_[i2];
    const glm::vec3& W3 = pathPoints_[i3];

    glm::vec3 T1 = 0.5f * (W2 - W0);
    glm::vec3 T2 = 0.5f * (W3 - W1);

    const float s = 1.0f / 3.0f;

    glm::vec3 B0 = W1;
    glm::vec3 B3 = W2;
    glm::vec3 B1 = B0 + T1 * s;
    glm::vec3 B2 = B3 - T2 * s;

    glm::vec3 pos = transform::bezier3(B0, B1, B2, B3, currentT_);
    glm::vec3 forward = transform::bezier3Tangent(B0, B1, B2, B3, currentT_);

    if (glm::length(forward) < 1e-6f) {
        forward = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    else {
        forward = glm::normalize(forward);
    }

    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
    if (glm::abs(glm::dot(forward, worldUp)) > 0.99f) {
        worldUp = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    glm::vec3 right = glm::normalize(glm::cross(worldUp, forward));
    glm::vec3 up = glm::normalize(glm::cross(forward, right));

    glm::mat4 R(1.0f);

    R[0] = glm::vec4(-forward, 0.0f);
    R[1] = glm::vec4(up, 0.0f);
    R[2] = glm::vec4(right, 0.0f);

    glm::mat4 M = glm::translate(glm::mat4(1.0f), pos) * R;

    if (bezierNode_) {
        bezierNode_->setLocal(M);
    }
}

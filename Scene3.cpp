// Scene3.cpp
#include "Scene3.h"

#include <GL/glew.h>
#include <cstdio>
#include <algorithm>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>



void Scene3::init(Scene& scene,
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
    ShaderProgram* phong)
{
    scene_ = &scene;
    phongShader_ = phong;

    meshSphere_ = &meshSphere;
    meshGround_ = &meshGround;
    meshTree_ = &meshTree;
    meshBush_ = &meshBush;
    meshShrek_ = &meshShrek;
    meshFiona_ = &meshFiona;
    meshToiled_ = &meshToiled;
    meshMushroom_ = &meshMushroom;

    shrekTexPaths_ = &shrekTexPaths;
    fionaTexPaths_ = &fionaTexPaths;
    toiledTexPaths_ = &toiledTexPaths;
    mushroomTexPaths_ = &mushroomTexPaths;

    auto root = SceneNode::makeGroup();

    lightsScene3_ = {
        Light::makePoint({-2.0f, 0.2f,  1.5f},  {0.90f, 1.00f, 0.40f}, 0.5f, {1.0f, 1.4f, 3.5f}),
        Light::makePoint({ 1.0f, 0.4f, -1.0f},  {0.90f, 1.00f, 0.40f}, 0.5f, {1.0f, 1.4f, 3.5f}),
        Light::makePoint({ 2.8f, 0.4f,  2.0f},  {0.90f, 1.00f, 0.40f}, 0.5f, {1.0f, 1.4f, 3.5f}),
        Light::makePoint({-3.0f, 0.6f, -2.5f},  {0.90f, 1.00f, 0.40f}, 0.5f, {1.0f, 1.4f, 3.5f}),
        Light::makePoint({ 0.0f, 0.7f,  3.2f},  {0.90f, 1.00f, 0.40f}, 0.5f, {1.0f, 1.4f, 3.5f}),
        Light::makePoint({-1.5f, 0.3f, -3.2f},  {0.90f, 1.00f, 0.40f}, 0.5f, {1.0f, 1.4f, 3.5f}),
        Light::makePoint({ 3.5f, 0.5f, -1.5f},  {0.90f, 1.00f, 0.40f}, 0.5f, {1.0f, 1.4f, 3.5f}),
        Light::makePoint({-2.5f, 0.4f, -0.5f},  {0.90f, 1.00f, 0.40f}, 0.5f, {1.0f, 1.4f, 3.5f}),
        Light::makePoint({ 2.0f, 0.6f, -2.0f},  {0.90f, 1.00f, 0.40f}, 0.5f, {1.0f, 1.4f, 3.5f}),
        Light::makePoint({ 0.5f, 0.5f,  0.5f},  {0.90f, 1.00f, 0.40f}, 0.5f, {1.0f, 1.4f, 3.5f})
      //  Light::makeAmbient({0.01f, 0.01f, 0.02f}, 0.1f)
    };

    fireflyPos_.resize(lightsScene3_.size());
    fireflyVel_.resize(lightsScene3_.size(), glm::vec3(0.0f));
    fireflyNodes_.resize(lightsScene3_.size(), nullptr);

    for (size_t i = 0; i < lightsScene3_.size(); ++i) {
        if (lightsScene3_[i].type == LightType::Ambient) continue;
        fireflyPos_[i] = lightsScene3_[i].position;

        SceneNode* n = scene_->addDrawableObject(
            root.get(),
            *meshSphere_,
            fireflyPos_[i],
            { 0.025f, 0.025f, 0.025f },
            { 0.90f, 1.00f, 0.40f, 1 },
            phongShader_
        );

        if (n && n->drawable())
            n->drawable()->setShadeModel(0); 
        fireflyNodes_[i] = n;
    }   

    // --- zem + textura ---
    Material matGround{ 0.02f, 0.25f, 0.08f, 1.0f };
    auto groundNode = scene_->addDrawableObject(
        root.get(), *meshGround_,
        { 0,0,0 }, { 1,1,1 },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        phongShader_, matGround
    );
    //{ 0.12f, 0.07f, 0.04f, 1.0f }
    GLuint texGround = TextureLoader::loadTexture2D("Models/ground.png");
    if (groundNode && groundNode->drawable()) {
        groundNode->drawable()->setTexture(texGround);
    }

    // --- stromy ---
    std::mt19937 rng(12345);
    std::uniform_real_distribution<float> distR(1.0f, 12.0f);
    std::uniform_real_distribution<float> distAng(0.0f, glm::two_pi<float>());
    std::uniform_real_distribution<float> distS(0.75f, 1.35f);

    Material noshine{ 0.3f, 1.0f, 0.05f, 4.0f };
    const int TREE_COUNT = 100;
    for (int i = 0; i < TREE_COUNT; ++i) {
        float r = distR(rng);
        float ang = distAng(rng);
        float sx = distS(rng);

        glm::vec3 pos{
            std::cos(ang) * r,
            -0.05f,
            std::sin(ang) * r
        };

        scene_->addDrawableObject(
            root.get(),
            *meshTree_,
            pos,
            { sx, sx, sx },
            { 0.002f, 0.08f, 0.02f, 1.0f },
            phongShader_, noshine
        );
    }

    // --- keře ---
    const int BUSH_COUNT = 50;
    for (int i = 0; i < BUSH_COUNT; ++i) {
        float r = distR(rng);
        float ang = distAng(rng);
        float sx = distS(rng);

        glm::vec3 pos{
            std::cos(ang) * r,
            -0.01f,
            std::sin(ang) * r
        };

        scene_->addDrawableObject(
            root.get(),
            *meshBush_,
            pos,
            { sx, sx, sx },
            { 0.01f, 0.18f, 0.05f, 1.0f },
            phongShader_
        );
    }

    // --- Shrek ---
    auto shrekNode = scene_->addDrawableObject(
        root.get(),
        *meshShrek_,
        { 4.0f, 0.0f, 0.0f },
        { 0.3f, 0.3f, 0.3f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        phongShader_, noshine
    );
    GLuint texShrek = 0;
    if (shrekTexPaths_ && !shrekTexPaths_->empty())
        texShrek = TextureLoader::loadTexture2D((*shrekTexPaths_)[0].c_str());
    if (shrekNode && shrekNode->drawable()) {
        shrekNode->drawable()->setTexture(texShrek);
        shrekNode->drawable()->setShadeModel(1);
    }

    // --- Fiona ---
    auto fionaNode = scene_->addDrawableObject(
        root.get(),
        *meshFiona_,
        { 5.0f, 0.0f, 0.0f },
        { 0.3f, 0.3f, 0.3f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        phongShader_, noshine
    );
    GLuint texFiona = 0;
    if (fionaTexPaths_ && !fionaTexPaths_->empty())
        texFiona = TextureLoader::loadTexture2D((*fionaTexPaths_)[0].c_str());
    if (fionaNode && fionaNode->drawable()) {
        fionaNode->drawable()->setTexture(texFiona);
        fionaNode->drawable()->setShadeModel(1);
    }

    // --- Toiled ---
    auto toiledNode = scene_->addDrawableObject(
        root.get(),
        *meshToiled_,
        { 4.5f, 0.0f, -1.0f },
        { 0.3f, 0.3f, 0.3f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        phongShader_, noshine
    );
    GLuint texToiled = 0;
    if (toiledTexPaths_ && !toiledTexPaths_->empty())
        texToiled = TextureLoader::loadTexture2D((*toiledTexPaths_)[0].c_str());
    if (toiledNode && toiledNode->drawable()) {
        toiledNode->drawable()->setTexture(texToiled);
        toiledNode->drawable()->setShadeModel(1);
    }

    // --- houby ---
    const int MUSHROOM_COUNT = 25;
    std::uniform_real_distribution<float> distR_M(1.0f, 9.0f);
    std::uniform_real_distribution<float> distAng_M(0.0f, glm::two_pi<float>());
    std::uniform_real_distribution<float> distScale_M(0.01f, 0.015f);

    Material mushroomMat{ 0.12f, 0.50f, 0.05f, 8.0f };

    for (int i = 0; i < MUSHROOM_COUNT; ++i)
    {
        float r = distR_M(rng);
        float ang = distAng_M(rng);
        float s = distScale_M(rng);

        glm::vec3 pos{
            std::cos(ang) * r,
            -0.04f,
            std::sin(ang) * r
        };

        SceneNode* mush = scene_->addDrawableObject(
            root.get(),
            *meshMushroom_,
            pos,
            { s, s, s },
            { 1.0f, 1.0f, 1.0f, 1.0f },
            phongShader_,
            mushroomMat
        );

        if (mush && mush->drawable()) {
            GLuint tex = 0;
            if (mushroomTexPaths_ && !mushroomTexPaths_->empty())
                tex = TextureLoader::loadTexture2D((*mushroomTexPaths_)[0].c_str());
            mush->drawable()->setTexture(tex);
            mush->drawable()->setShadeModel(1);
        }
        if (mush)
            mushroomNodes_.push_back(mush);
    }

    scene_->addRoot(std::move(root));
}

void Scene3::update(float dt)
{
    updateFireflies_(dt);
}

void Scene3::onPick(SceneNode*& selectedNode,
    const glm::vec3& lastClickWorldPos,
    const glm::vec3& cameraPos)
{
    if (!selectedNode)
        return;

    auto it = std::find(mushroomNodes_.begin(), mushroomNodes_.end(), selectedNode);
    if (it == mushroomNodes_.end()) {
        return;
    }

    const float dist = glm::length(cameraPos - lastClickWorldPos);
    const float PICKUP_DIST = 1.0f;

    if (dist <= PICKUP_DIST) {
        if (auto* d = selectedNode->drawable()) {
            d->setVisible(false); 
        }
        mushroomsCollected_++;
        std::printf("Mushroom collected! total=%d/20 \n", mushroomsCollected_);

        mushroomNodes_.erase(it);
        selectedNode = nullptr;
    }
    else {
        std::printf("Too far to pick mushroom (dist=%.2f, max=%.2f)\n",
            dist, PICKUP_DIST);
        selectedNode = nullptr;
    }

    if (mushroomsCollected_ >= 20) {
        std::printf("\n");
        std::printf("=========================================\n");
        std::printf("                   VÝHRA!                \n");
        std::printf("=========================================\n");
        std::printf("  Gratulujeme! Posbíral jsi všechny\n");
        std::printf("  houby ve třetí scéně.\n");
        std::printf("\n");
        std::printf("  Světlušky ti děkují za pomoc a\n");
        std::printf("  noc v lese znovu září klidem.\n");
        std::printf("=========================================\n\n");
    }
}

void Scene3::updateFireflies_(float dt)
{
    std::normal_distribution<float> n01(0.0f, 1.0f);

    const float SPEED = 0.7f;
    const float JITTER_STRENGTH = 0.6f;

    for (size_t i = 0; i < lightsScene3_.size(); ++i) {
        if (lightsScene3_[i].type == LightType::Ambient)
            continue;

        glm::vec3 jitter(
            n01(rngFireflies_) * JITTER_STRENGTH,
            n01(rngFireflies_) * (JITTER_STRENGTH * 0.7f),
            n01(rngFireflies_) * JITTER_STRENGTH
        );

        fireflyVel_[i] += jitter * dt;

        float len = glm::length(fireflyVel_[i]);
        if (len < 1e-4f) {
            fireflyVel_[i] = glm::vec3(1.0f, 0.0f, 0.0f);
            len = 1.0f;
        }
        fireflyVel_[i] = (fireflyVel_[i] / len) * SPEED;

        fireflyPos_[i] += fireflyVel_[i] * dt;

        fireflyPos_[i].y = glm::clamp(fireflyPos_[i].y, 0.25f, 0.9f);
        if (fireflyPos_[i].x > 6.0f) { fireflyPos_[i].x = 6.0f; fireflyVel_[i].x = -fireflyVel_[i].x; }
        if (fireflyPos_[i].x < -6.0f) { fireflyPos_[i].x = -6.0f; fireflyVel_[i].x = -fireflyVel_[i].x; }
        if (fireflyPos_[i].z > 6.0f) { fireflyPos_[i].z = 6.0f; fireflyVel_[i].z = -fireflyVel_[i].z; }
        if (fireflyPos_[i].z < -6.0f) { fireflyPos_[i].z = -6.0f; fireflyVel_[i].z = -fireflyVel_[i].z; }

        lightsScene3_[i].position = fireflyPos_[i];

        if (fireflyNodes_[i]) {
            if (auto* d = fireflyNodes_[i]->drawable()) {
                glm::mat4 M(1.0f);
                M = glm::translate(M, fireflyPos_[i]);
                M = glm::scale(M, glm::vec3(0.025f));
                d->setModel(M);
            }
        }
    }
}

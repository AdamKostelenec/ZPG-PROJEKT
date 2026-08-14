// Scene2.cpp
#include "Scene2.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include "TextureLoader.h"

void Scene2::init(Scene& scene,
    const Mesh& meshSphere,
    ShaderProgram* phong)
{
    scene_ = &scene;
    meshSphere_ = &meshSphere;
    phongShader_ = phong;

    const float R = 0.60f;
    const float D = 2.2f * R;

    auto root = SceneNode::makeGroup();

    lightsScene2_.clear();
    lightsScene2_ = {
        Light::makePoint({ 0,0,0 }, { 1,1,1 }, 1.2f, { 1,0,0 }),
        Light::makeAmbient({0.7f, 0.7f, 0.7f}, 1.0f)
    };

    scene_->addDrawableObject(root.get(), *meshSphere_,
        { 0.0f, +D, 0.0f }, { 1,1,1 }, { 0,0.5f,0.85f,1 }, phongShader_);
    scene_->addDrawableObject(root.get(), *meshSphere_,
        { 0.0f, -D, 0.0f }, { 1,1,1 }, { 0,0.5f,0.85f,1 }, phongShader_);
    scene_->addDrawableObject(root.get(), *meshSphere_,
        { +D, 0.0f, 0.0f }, { 1,1,1 }, { 0,0.5f,0.85f,1 }, phongShader_);
    scene_->addDrawableObject(root.get(), *meshSphere_,
        { -D, 0.0f, 0.0f }, { 1,1,1 }, { 0,0.5f,0.85f,1 }, phongShader_);
   
    scene_->addRoot(std::move(root));
}

void Scene2::update(float dt)
{
    time_ += dt;
   
}

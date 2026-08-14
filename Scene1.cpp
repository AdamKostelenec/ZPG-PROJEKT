#include "Scene1.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

void Scene1::init(Scene& scene,
    const Mesh& meshTri,
    const Mesh& meshKos,
    ShaderProgram* phong)
{
    scene_ = &scene;
    phongShader_ = phong;
    meshTri_ = &meshTri;
    meshKos_ = &meshKos;

    lightsScene1_.clear();
    lightsScene1_.push_back(
        Light::makePoint({ 1.5f, 1.5f, 1.5f }, { 1,1,1 }, 1.0f)
    );

    auto root = SceneNode::makeGroup();

    auto rotNode = SceneNode::makeGroup();
    s1_rotNode_ = rotNode.get();       
    root->addChild(std::move(rotNode));

    scene_->addDrawableObject(
        s1_rotNode_,
        *meshTri_,
        { 0,0,0 },
        { 1,1,1 },
        { 1,0,0,1 },
        phongShader_
    );
    
    auto loginSpin = SceneNode::makeGroup();
    loginSpin->setLocal(glm::translate(glm::mat4(1.0f),
        glm::vec3(0, 2, -5)));
    loginSpin_ = loginSpin.get();
	Material ShineLogin{ 0.15f, 0.25f, 2.0f, 256 };
    SceneNode* login=scene_->addDrawableObject(
        loginSpin_,
        *meshKos_,
        { 0,0,0 },
        { 1,1,1 },
        { 0.95f, 0.95f, 0.98f, 1.0f },
        phongShader_,ShineLogin
    );
    root->addChild(std::move(loginSpin));
    scene_->addRoot(std::move(root));
}

void Scene1::update(float dt)
{
    if (s1_rotNode_) {
        glm::mat4 R = glm::rotate(glm::mat4(1.0f), dt * 0.8f, glm::vec3(0, 0, 1));
        s1_rotNode_->setLocal(R * s1_rotNode_->local());
    }
    if (loginSpin_)
    {
		glm::mat4 R = glm::rotate(glm::mat4(1.0f), dt * 0.2f, glm::vec3(0, 1, 0));
		loginSpin_->setLocal(loginSpin_->local()*R);
    }
}

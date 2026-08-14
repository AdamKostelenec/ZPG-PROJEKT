#include "Scene6.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

void Scene6::init(Scene& scene,
	const Mesh& meshSphere,
	ShaderProgram* phong)
{
	scene_ = &scene;
	meshSphere_ = &meshSphere;
	phongShader_ = phong;

	const float R = 0.60f;
	const float D = 2.2f * R;

	auto root = SceneNode::makeGroup();
	lightsScene6_.clear();
	lightsScene6_.push_back(
		Light::makePoint({ 0,0,0 }, { 1,1,1 }, 1.2f, { 1,0,0 })
	);
	SceneNode* topNode = scene_->addDrawableObject(root.get(), *meshSphere_,
		{ 0.0f, +D, 0.0f }, { 1,1,1 }, { 0,0.5f,0.85f,1 }, phongShader_);
	topNode->drawable()->setShadeModel(0); // constant

	SceneNode* downNode = scene_->addDrawableObject(root.get(), *meshSphere_,
		{ 0.0f, -D, 0.0f }, { 1,1,1 }, { 0,0.5f,0.85f,1 }, phongShader_);
	downNode->drawable()->setShadeModel(1); // lambert

	SceneNode* rightNode = scene_->addDrawableObject(root.get(), *meshSphere_,
		{ +D , 0.0f, 0.0f }, { 1,1,1 }, { 0,0.5f,0.85f,1 }, phongShader_);
	rightNode->drawable()->setShadeModel(2); //phong

	SceneNode* leftNode = scene_->addDrawableObject(root.get(), *meshSphere_,
		{ -D, 0.0f, 0.0f }, { 1,1,1 }, { 0,0.5f,0.85f,1 }, phongShader_);
	leftNode->drawable()->setShadeModel(3); // blinn-phong
	scene_->addRoot(std::move(root));
}
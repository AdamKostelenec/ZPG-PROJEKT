#include "Scene7.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "TextureLoader.h"

void Scene7::init(Scene& scene,
	const Mesh& meshSphere,
	ShaderProgram* phong)
{
	scene_ = &scene;
	meshSphere_ = &meshSphere;
	phongShader_ = phong;


	lightsScene7_.clear();
	lightsScene7_.push_back(
		Light::makePoint({ 0,0,0 }, { 1,1,1 }, 1.4f, { 1,0,0 })
	);
	auto root = SceneNode::makeGroup();

	SceneNode* centerNode = scene_->addDrawableObject(
		root.get(),
		*meshSphere_,
		{ 0.0f, 0.0f, 0.0f },
		{ 1,1,1 },
		{ 1.0f, 1.0f, 1.0f, 1.0f }, phongShader_);
	GLuint textSun = TextureLoader::loadTexture2D("Models/sun.jpg");

	centerNode->drawable()->setShadeModel(0);
	centerNode->drawable()->setTexture(textSun);

	auto uzel = SceneNode::makeGroup();
	uzel_ = uzel.get();

	auto radiusNode = SceneNode::makeGroup();
	radiusNode->setLocal(glm::translate(glm::mat4(1.0f),
		glm::vec3(5.0f, 0.0f, 0.0f)));

	auto rotatesphere = SceneNode::makeGroup();
	rotate_ = rotatesphere.get();

	SceneNode* prvninode = scene_->addDrawableObject(
		rotatesphere.get(),
		*meshSphere_,
		{ 0.0f, 0.0f, 0.0f },
		{ 1,1,1 },
		{ 1.0f, 1.0f, 1.0f, 1.0f }, phongShader_);
	GLuint textEarth = TextureLoader::loadTexture2D("Models/zeme.jpg");
	if (prvninode && prvninode->drawable()) {
		prvninode->drawable()->setTexture(textEarth);
	}


	auto marsorbit = SceneNode::makeGroup();
	marsorbit_ = marsorbit.get();

	auto marsradius = SceneNode::makeGroup();
	marsradius->setLocal(glm::translate(glm::mat4(1.0f),
		glm::vec3(8.0f, 0.0f, 0.0f)));

	auto marsspin = SceneNode::makeGroup();
	marsspin_ = marsspin.get();

	SceneNode* marsnode = scene_->addDrawableObject(
		marsspin.get(),
		*meshSphere_,
		{ 0.0f, 0.0f, 0.0f },
		{ 0.53f,0.53f,0.53f },
		{ 1.0f, 1.0f, 1.0f, 1.0f }, phongShader_);

	GLuint texMars = TextureLoader::loadTexture2D("Models/mars.jpg");
	if (marsnode && marsnode->drawable()) {
		marsnode->drawable()->setTexture(texMars);
	}



	auto jupiterorbit = SceneNode::makeGroup();
	jupiterorbit_ = jupiterorbit.get();
	

	auto jupiterradius = SceneNode::makeGroup();
	jupiterradius->setLocal(glm::translate(glm::mat4(1.0f),
		glm::vec3(12.0f, 0.0f, 0.0f)));

	auto jupiterspin = SceneNode::makeGroup();
	jupiterspin_ = jupiterspin.get();


	SceneNode* jupiternode = scene_->addDrawableObject(
		jupiterspin.get(),
		*meshSphere_,
		{ 0.0f, 0.0f, 0.0f },
		{ 2.33f,2.33f,2.33f },
		{ 1.0f, 1.0f, 1.0f, 1.0f }, phongShader_);

	GLuint texJupiter = TextureLoader::loadTexture2D("Models/jupiter.jpg");
	if (jupiternode && jupiternode->drawable()) {
		jupiternode->drawable()->setTexture(texJupiter);
	}

	jupiterradius->addChild(std::move(jupiterspin));
	jupiterorbit->addChild(std::move(jupiterradius));
	root->addChild(std::move(jupiterorbit));

	marsradius->addChild(std::move(marsspin));
	marsorbit->addChild(std::move(marsradius));
	root->addChild(std::move(marsorbit));

	radiusNode->addChild(std::move(rotatesphere));
	uzel->addChild(std::move(radiusNode));
	root->addChild(std::move(uzel));
	scene_->addRoot(std::move(root));
}

void Scene7::update(float dt)
{
	time_ += dt;

	glm::mat4 rotace = glm::rotate(glm::mat4(1.0f),
		time_ * 0.25f,
		glm::vec3(0, 1, 0));
		uzel_->setLocal(rotace);

	glm::mat4 rotacesphere = glm::rotate(glm::mat4(1.0f),
		time_ * 1.0f,
		glm::vec3(0, 1, 0));
	rotate_->setLocal(rotacesphere);


	glm ::mat4 marsorbitrot = glm::rotate(glm::mat4(1.0f),
		time_ * 0.15f,
		glm::vec3(0, 1, 0));

	marsorbit_->setLocal(marsorbitrot);

	glm::mat4 marsspinrot = glm::rotate(glm::mat4(1.0f),
		time_ * 0.8f,
		glm::vec3(0, 1, 0));
	marsspin_->setLocal(marsspinrot);

	glm::mat4 jupiterrot = glm::rotate(glm::mat4(1.0f),
		time_ * 0.1f,
		glm::vec3(0, 1, 0));

	jupiterorbit_->setLocal(jupiterrot);

	glm::mat4 jupiterspinrot = glm::rotate(glm::mat4(1.0f),
		time_ * 0.5f,
		glm::vec3(0, 1, 0));

	jupiterspin_->setLocal(jupiterspinrot);

}
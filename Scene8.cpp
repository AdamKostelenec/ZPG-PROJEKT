#include "Scene8.h"



void Scene8::init(Scene& scene,
	const Mesh& meshSphere, const Mesh& meshGround, const Mesh& meshFormule,
	ShaderProgram* phong)
{
	scene_ = &scene;
	phongShader_ = phong;
	
	auto root = SceneNode::makeGroup();
	lightsScene8_.clear();
	lightsScene8_.push_back(
		Light::makeDirectional({ 0.0f, -1.0f, 0.0f }, { 1,1,1 }, 0.4f)
	);
	

	SceneNode* zem = scene_->addDrawableObject(
		root.get(),
		meshGround,
		{ 0.0f, 0.0f, 0.0f },
		{ 1,1,1 },
		{ 1.0f, 1.0f, 1.0f, 1.0f }, phongShader_);


		formuleNode_ = scene_->addDrawableObject(
		root.get(),
		meshFormule,
		formulePos_,
		formuleScale_,
		{ 1.0f, 0.0f, 0.0f, 1.0f }, phongShader_);

	scene_->addRoot(std::move(root));

}

void Scene8::update(float dt, bool up, bool down, bool left, bool right)
{
	time_ += dt;

	const float moveSpeed = 6.0f;
	const float turnSpeed = 1.0f;
	if (left && up || right &&down)
	{
		std::printf("Left pressed\n");
		yaw_ += turnSpeed * dt;
	}	
	if(right && up || left && down)
	{
		std::printf("Right pressed\n");
		yaw_ -= turnSpeed * dt;
	}
	glm::mat4 R = glm::rotate(glm::mat4(1.0f), yaw_, glm::vec3(0, 1, 0));
	const float yawFix = glm::radians(90.0f);

	glm::vec3 forward = glm::normalize(glm::vec3(R * glm::vec4(-1, 0, 0, 0)));

	if (up)
	{
		std::printf("Up pressed\n");
		formulePos_ += forward * moveSpeed * dt;
	}

	if (down)
	{
		std::printf("Down pressed\n");
		formulePos_ -= forward * moveSpeed * dt;

	}
	
	glm::mat4 T = glm::translate(glm::mat4(1.0f), formulePos_);
	glm::mat4 S = glm::scale(glm::mat4(1.0f), { 1,1,1 });

	formuleNode_->setLocal(T * R * S);
}
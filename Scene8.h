#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "ShaderProgram.h"
#include "Draweble.h"


class Scene8 {
public:

	Scene8() = default;
	void init(Scene& scene,
		const Mesh& meshSphere, const Mesh& meshGround, const Mesh& meshFormule,
		ShaderProgram* phong);
	void update(float dt, bool up, bool down, bool left, bool right);


	const std::vector<Light>& lights() const { return lightsScene8_; }

private:
	Scene* scene_ = nullptr;
	ShaderProgram* phongShader_ = nullptr;
	const Mesh* meshSphere_ = nullptr;
	const Mesh* meshGround_ = nullptr;
	const Mesh* meshFormule_ = nullptr;
	SceneNode* formuleNode_ = nullptr;


	glm::vec3 formulePos_{ 0.0f, 0.0f, 0.0f };
	float yaw_ = 0.0f;             
	glm::vec3 formuleScale_{ 0.1f,0.1f,0.1f };

	float time_ = 0.0f;
	std::vector<Light> lightsScene8_;
};
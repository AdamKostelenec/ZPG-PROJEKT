#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Mesh.h"
#include "Light.h"

#include "ShaderProgram.h"
#include "Draweble.h"

class Scene7 {
public:
	Scene7() = default;
	void init(Scene& scene,
		const Mesh& meshSphere,
		ShaderProgram* phong);

	void update(float dt);

	const std::vector<Light>& lights() const { return lightsScene7_; }

private:
	Scene* scene_ = nullptr;
	ShaderProgram* phongShader_ = nullptr;
	const Mesh* meshSphere_ = nullptr;
	SceneNode* rotate_ = nullptr;
	SceneNode* uzel_ = nullptr;
	SceneNode* radius_ = nullptr;

	SceneNode* marsspin_ = nullptr;
	SceneNode* marsorbit_ = nullptr;


	SceneNode* jupiterorbit_ = nullptr;
	SceneNode* jupiterspin_ = nullptr;

	float time_ = 0.0f;

	std::vector<Light> lightsScene7_;

};
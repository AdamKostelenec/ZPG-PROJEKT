#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Scene.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Draweble.h"
#include "Transform.h"
#include "Light.h"

class Scene1 {
public:
	Scene1() = default;
	void init(Scene& scene,
		const Mesh& meshTri,
		const Mesh& meshKos,
		ShaderProgram* phong);
	void update(float dt);
	const std::vector<Light>& lights() const { return lightsScene1_; }
private:

	Scene* scene_ = nullptr;
	ShaderProgram* phongShader_ = nullptr;
	SceneNode* s1_rotNode_ = nullptr;
	const Mesh* meshTri_ = nullptr;
	const Mesh* meshKos_ = nullptr;
	std::vector<Light> lightsScene1_;

	SceneNode* loginSpin_ = nullptr;

};
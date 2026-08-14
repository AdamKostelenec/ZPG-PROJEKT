// Application.h
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <array>
#include <random>

#include "skycube.h"
#include "Controller.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Light.h"
#include "Primitives.h"
#include "Draweble.h"
#include "ObjLoader.h"
#include "Transform.h"
#include "Models/tree.h"
#include "Models/bushes.h"
#include "Models/gift.h"
#include "Models/suzi_smooth.h"
#include "Models/suzi_flat.h"
#include "Models/sphere.h"
#include "Models/triangle.h"
#include "TextureLoader.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"   
#include "Scene4.h"
#include "Scene5.h"
#include "Scene6.h"
#include "Scene7.h"
#include "Scene8.h"

class Application {
public:
    Application(int width, int height, const std::string& title);
    ~Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    void run(const std::function<void()>& renderFunc);

    void initGame();
    void run();

    GLFWwindow* getWindow() const { return window; }

private:

    glm::vec3 lastClickWorldPos_{ 0.0f, 0.0f, 0.0f };

    // --- okno / GL ---
    GLFWwindow* window = nullptr;
    int width, height;
    std::string title;

    void initGLFW();
    void initGLEW();
    void setupCallbacks();

    // --- vstupy + kamera ---
    std::unique_ptr<Controller> controller_;

    // --- scény ---
    std::vector<std::unique_ptr<Scene>> scenes_;
	std::unique_ptr<Scene1> scene1_;
    std::unique_ptr<Scene2> scene2_;
    std::unique_ptr<Scene3> scene3_;
    std::unique_ptr<Scene4> scene4_;
	std::unique_ptr<Scene5> scene5_;
	std::unique_ptr<Scene6> scene6_;
	std::unique_ptr<Scene7> scene7_;
	std::unique_ptr<Scene8> scene8_;




    std::unique_ptr<ShaderProgram> phongShader_;

    std::unique_ptr<ShaderProgram> skyboxShader_;


    // --- SKYBOX ---
    Mesh   meshSkybox_;
    std::array<GLuint, 7> skyboxTex_{};

    Mesh meshTree_;
    Mesh meshBush_;
    Mesh meshGift_;
    Mesh meshSuziSmooth_;
    Mesh meshSuziFlat_;
    Mesh meshTri_;
    Mesh meshSphere_;
    Mesh meshGround_;
    Mesh meshFormule_;
	Mesh meshShrek_;
	Mesh meshFiona_;    
	Mesh meshToiled_;
	Mesh meshMushroom_;
    Mesh meshKos_;
        std::vector<std::string> shrekTexPaths;
		std::vector<std::string> fionaTexPaths;
		std::vector<std::string> toiledTexPaths;
		std::vector<std::string> mushroomTexPaths;
        std::vector<std::string> treeTexPaths;



    // --- světlušky ---
    std::vector<glm::vec3> fireflyPos_;
    std::vector<glm::vec3> fireflyVel_;
    std::vector<SceneNode*> fireflyNodes_;



   
    // --- game ---
    std::vector<SceneNode*> mushroomNodes_;
    int mushroomsCollected_ = 0;



	//selected object for manipulation
    SceneNode* selectedNode_ = nullptr;
	SceneNode* beforeNode_ = nullptr;



    // RNG
    std::mt19937 rngFireflies_{ 1337 };

    // build funkce
    void createShaders_();
    void createMeshes_();
    void createScenes_();

    glm::vec3 unprojectAt(int x, int y, float depth, const FrameState& st);
    void handlePickedObject(SceneNode* picked,
        float depth,
        int x, int y,
        const FrameState& st);
    void frame_();
};

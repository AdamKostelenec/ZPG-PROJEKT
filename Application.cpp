#include "Application.h"
#include <stdexcept>
#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Application::Application(int width, int height, const std::string& title)
    : width(width), height(height), title(title)
{
    initGLFW();
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    initGLEW();
    setupCallbacks();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
}

Application::~Application() {
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::initGLFW() {
    if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
}

void Application::initGLEW() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) throw std::runtime_error("Failed to initialize GLEW");
}

void Application::setupCallbacks() {
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
        });
}

void Application::run(const std::function<void()>& renderFunc) {
    while (!glfwWindowShouldClose(window)) {
        renderFunc();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


//======================================================
glm::vec3 Application::unprojectAt(int x, int y, float depth, const FrameState& st)
{
    glm::vec3 point((float)x, (float)y, depth);
    glm::ivec4 viewport(0, 0, st.fbw, st.fbh);
    glm::vec3 worldPos = glm::unProject(point, st.view, st.proj, viewport);

    std::printf("unProject: [%f, %f, %f]\n", worldPos.x, worldPos.y, worldPos.z);
    return worldPos;
}

// =====================================================
void Application::initGame()
{
    std::printf("OpenGL  : %s\n", glGetString(GL_VERSION));
    std::printf("GLSL    : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    std::printf("Vendor  : %s\n", glGetString(GL_VENDOR));
    std::printf("Renderer: %s\n", glGetString(GL_RENDERER));

    std::printf("\n");
    std::printf("=========================================\n");
    std::printf("            OVLÁDÁNÍ APLIKACE            \n");
    std::printf("=========================================\n");
    std::printf(" Myš + Pravé tlačítko  : Otočení kamerou\n");
    std::printf(" Levé tlačítko myši    : Picking objektu\n");
    std::printf(" WASD                  : Pohyb hráče / kamery\n");
    std::printf(" C                     : Free-move kamera ON/OFF\n");
    std::printf(" F                     : Baterka ON/OFF\n");
    std::printf(" 1–8                   : Přepínání scén\n");
    std::printf("\n");
    std::printf(" --- Editace objektů ---\n");
    std::printf(" I                     : Insert mode ON/OFF\n");
    std::printf(" M                     : Move mode ON/OFF\n");
    std::printf(" Delete                : Delete mode ON/OFF\n");
    std::printf("\n");
    std::printf(" --- Bézier mód (scéna 5) ---\n");
    std::printf(" B                     : Bezier edit mode ON/OFF\n");
    std::printf(" Levé tlačítko myši    : Přidání bodu křivky (v Bezier modu)\n");
    std::printf(" R                     : Reset bodů křivky (v Bezier modu)\n");
    std::printf("=========================================\n\n");


    controller_ = std::make_unique<Controller>(window);
    controller_->initializeCallbacks(true);

    createShaders_();
    createMeshes_();
    createScenes_();
}

void Application::run()
{
    while (!glfwWindowShouldClose(window)) {
        frame_();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

// ----------------- SHADERY -----------------
void Application::createShaders_()
{
    phongShader_ = std::make_unique<ShaderProgram>(
        ShaderProgram::fromFiles("lit.vert", "phong.frag")
    );
    skyboxShader_ = std::make_unique<ShaderProgram>(
		ShaderProgram::fromFiles("skybox.vert", "skybox.frag"));
    ShaderProgram::UniformNames un;
    un.uView = "uView";
    un.uProj = "uProj";
    un.uCamPos = "uCamPos";
    un.uViewProj = nullptr;

    phongShader_->setCameraUniformNames(un);
    controller_->addObserver(phongShader_.get());
}

// ----------------- MESHE -----------------
void Application::createMeshes_()
{
    const float R = 0.60f;
    const float D = 2.2f * R;

    meshTree_ = makeMeshWithNormals(tree, std::size(tree), 0.25f);
    meshBush_ = makeMeshWithNormals(bushes, std::size(bushes), 0.90f);
    meshGift_ = makeMeshWithNormals(gift, std::size(gift), 1.80f);
    meshSuziSmooth_ = makeMeshWithNormals(suziSmooth, std::size(suziSmooth), 0.35f);
    meshSuziFlat_ = makeMeshWithNormals(suziFlat, std::size(suziFlat), 0.35f);
    meshTri_ = makeMeshWithNormals(triangle, std::size(triangle), 1.0f);
    meshSphere_ = makeMeshWithNormalsUV(sphere, std::size(sphere), R);
    meshGround_ = makePlaneMeshPN(glm::vec3(0.0f, -0.05f, 0.0f), glm::vec2(50.0f, 50.0f));
	meshKos_ = loadOBJAsMesh("Models/KOS0409.obj", false, 1.0f);
    meshFormule_ = loadOBJAsMesh("Models/formula1.obj", true, 1.0f);
    meshShrek_ = loadOBJAsMesh("Models/shrek.obj", false, 1.0f, &shrekTexPaths);
    meshFiona_ = loadOBJAsMesh("Models/fiona.obj", false, 1.0f, &fionaTexPaths);
    meshToiled_ = loadOBJAsMesh("Models/toiled.obj", false, 1.0f, &toiledTexPaths);
    meshMushroom_ = loadOBJAsMesh("Models/mushroom/mushroom.obj", false, 1.0f, &mushroomTexPaths);

    {
        std::vector<float> skyVerts(
            skycube,
            skycube + sizeof(skycube) / sizeof(float)
        );
        meshSkybox_ = Mesh(skyVerts, 3);



        {
            std::array<std::string, 6> faces0 = {
                "cubemap/posx.jpg",  
                "cubemap/negx.jpg",    
                "cubemap/posy.jpg",   
                "cubemap/negy.jpg",
                "cubemap/posz.jpg",   
                "cubemap/negz.jpg"    
            };
            skyboxTex_[0] = TextureLoader::loadCubemap(faces0);
        }

        {
            std::array<std::string, 6> faces1 = {
                "cubemap/right.png",
                "cubemap/left.png",
                "cubemap/top.png",
                "cubemap/bottom.png",
                "cubemap/front.png",
                "cubemap/back.png"
            };
            skyboxTex_[0] = TextureLoader::loadCubemap(faces1);
            skyboxTex_[1] = TextureLoader::loadCubemap(faces1);
            skyboxTex_[2] = TextureLoader::loadCubemap(faces1);
            skyboxTex_[3] = TextureLoader::loadCubemap(faces1);
            skyboxTex_[4] = TextureLoader::loadCubemap(faces1);
			skyboxTex_[6] = TextureLoader::loadCubemap(faces1);
        }


    }
}

// ----------------- SCÉNY -----------------
void Application::createScenes_()
{
    scenes_.clear();
    scenes_.emplace_back(std::make_unique<Scene>());
    scenes_.emplace_back(std::make_unique<Scene>());
    scenes_.emplace_back(std::make_unique<Scene>());
    scenes_.emplace_back(std::make_unique<Scene>());
	scenes_.emplace_back(std::make_unique<Scene>());
	scenes_.emplace_back(std::make_unique<Scene>());
	scenes_.emplace_back(std::make_unique<Scene>());
	scenes_.emplace_back(std::make_unique<Scene>());

	scene1_ = std::make_unique<Scene1>();
    scene1_->init(
        *scenes_[0],
        meshTri_,
        meshKos_,
        phongShader_.get()
	);
    scene2_ = std::make_unique<Scene2>();
    scene2_->init(
        *scenes_[1],
        meshSphere_,
        phongShader_.get()
    );

    scene3_ = std::make_unique<Scene3>();
    scene3_->init(
        *scenes_[2],
        meshSphere_,
        meshGround_,
        meshTree_,
        meshBush_,
        meshShrek_,
        meshFiona_,
        meshToiled_,
        meshMushroom_,
        shrekTexPaths,
        fionaTexPaths,
        toiledTexPaths,
        mushroomTexPaths,
        phongShader_.get()
    );

    scene4_ = std::make_unique<Scene4>();
    scene4_->init(
        *scenes_[3],
        meshSphere_,
        phongShader_.get()
    );

    scene5_ = std::make_unique<Scene5>();
    scene5_->init(
        *scenes_[4],
        meshGround_,
        meshFormule_,
        phongShader_.get()
    );

	scene6_ = std::make_unique<Scene6>();
    scene6_->init(
        *scenes_[5],
        meshSphere_,
        phongShader_.get()
	);
	scene7_ = std::make_unique<Scene7>();
    scene7_->init(
        *scenes_[6],
        meshSphere_,
        phongShader_.get()
	);
	scene8_ = std::make_unique<Scene8>();
    scene8_->init(
        *scenes_[7],
        meshSphere_,meshGround_,meshFormule_,
        phongShader_.get()
	);


}




// ----------------- FRAME -----------------
void Application::frame_()
{
    FrameState st = controller_->update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   
   

    int idx = st.activeScene;
    if (idx < 0) idx = 0;
    if (idx >= (int)scenes_.size()) idx = (int)scenes_.size() - 1;

    if (skyboxShader_) {
        GLuint tex = 0;
        if (idx >= 0 && idx < (int)skyboxTex_.size()) {
            tex = skyboxTex_[idx];
        }

        if (tex != 0) {
            glDepthMask(GL_FALSE);
            glDepthFunc(GL_LEQUAL);

            skyboxShader_->use();

            glm::mat4 viewNoTrans = st.view;
            viewNoTrans[3][0] = 0.0f;
            viewNoTrans[3][1] = 0.0f;
            viewNoTrans[3][2] = 0.0f;

            skyboxShader_->setUniform("uView", viewNoTrans);
            skyboxShader_->setUniform("uProj", st.proj);
            skyboxShader_->setUniform("uSkybox", 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

            meshSkybox_.draw();

            ShaderProgram::unuse();
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS);
        }
    }


    ShaderProgram* activeProg = phongShader_.get();
    activeProg->use();

    activeProg->setUniform("uCamPos", controller_->camera().position());
    glm::mat4 uCustomToSend(1.0f);
    // uCustomToSend[3][3] = 5.0f;   
    activeProg->setUniform("uCustom", uCustomToSend);
    activeProg->setUniform("w", uCustomToSend[3][3]);


    std::vector<Light> currentLights;
	if (idx == 0 && scene1_) currentLights = scene1_->lights();
    else if (idx == 1 && scene2_) currentLights = scene2_->lights();
    else if (idx == 2 && scene3_) currentLights = scene3_->lights();
    else if (idx == 3 && scene4_) currentLights = scene4_->lights();
    else if (idx == 4 && scene5_) currentLights = scene5_->lights();
    else if (idx == 5 && scene6_) currentLights = scene6_->lights();
	else if (idx == 6 && scene7_) currentLights = scene7_->lights();
	else if (idx == 7 && scene8_) currentLights = scene8_->lights();

    if (idx == 4 && scene5_) {
        scene5_->update(st.dt, st.beziermode);
    }

    if (idx==0 && scene1_)
    {
        scene1_->update(st.dt);
    }

    if (idx==3&&scene4_) {
        scene4_->update(st.dt);
    }


    if (idx == 2 && scene3_) {
        scene3_->update(st.dt);
    }
    if (idx == 1 && scene3_) {
        scene2_->update(st.dt);
    }

    if(idx== 6 && scene7_) {
        scene7_->update(st.dt);
	}

    if (idx == 7 && scene8_) {
        scene8_->update(st.dt,st.uparrow,st.downarrow,st.leftarrow,st.rightarrow);
    }

    if (st.flashlightOn) {
        glm::vec3 camPos = controller_->camera().position();
        glm::vec3 camDir = controller_->camera().front();
        currentLights.push_back(
            Light::makeSpot(camPos, camDir, { 1,1,1}, 6.0f,
                glm::radians(20.0f), { 1.0f, 1.0f, 1.0f })
        );
    }

    if (!currentLights.empty()) uploadLights(*activeProg, currentLights);
    else activeProg->setUniform("uNumLights", 0);

    scenes_[idx]->draw(*activeProg, st.view, st.proj);
    ShaderProgram::unuse();

    if (st.pickrequest) {
        int x = st.pickx;
        int y = st.picky;
        int newy = st.fbh - y;

        GLubyte color[4];
        GLfloat depth = 1.0f;
        GLuint  index = 0;

        glReadPixels(x, newy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
        glReadPixels(x, newy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        glReadPixels(x, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

        std::printf(
            "Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
            x, y, color[0], color[1], color[2], color[3], depth, index
        );


        if (st.beziermode && idx == 4 && scene5_) {
            if (depth < 1.0f) {
                glm::vec3 worldPos = unprojectAt(x, newy, depth, st);
                worldPos.y = 0.0f;
                scene5_->addBezierPoint(worldPos);

                std::printf("Bezier point added at [%f, %f, %f]\n",
                    worldPos.x, worldPos.y, worldPos.z);
            }
            else {
                std::printf("Bezier: klik do pozadí, bod nepřidán (depth=1)\n");
            }
        }
        else {
            Scene* scene = scenes_[idx].get();
            if (SceneNode* picked = scene->nodeFromId(index)) {
                beforeNode_ = selectedNode_;
                selectedNode_ = picked;
                lastClickWorldPos_ = unprojectAt(x, newy, depth, st);
            }

            if (selectedNode_ && idx == 2 && scene3_) {
                const glm::vec3 camPos = controller_->camera().position();
                scene3_->onPick(selectedNode_, lastClickWorldPos_, camPos);
            }

        }
    }

    if (st.deletemode && selectedNode_) {
        SceneNode* toDelete = selectedNode_;
        selectedNode_ = nullptr;

        Scene* scene = scenes_[idx].get();
        if (scene && scene->deleteNode(toDelete)) {
            std::printf("Deleted node %p\n", (void*)toDelete);
        }
        else {
            std::printf("Failed to delete node %p (not found in scene)\n", (void*)toDelete);
        }
    }



    if (st.insertmode && selectedNode_ && idx == 2) {
        Material noshine{ 0.3f, 1.0f, 0.05f, 4.0f };
        scenes_[2]->addDrawableObject(
            nullptr, meshTree_,
            lastClickWorldPos_,
            { 1.0f, 1.0f, 1.0f },
            { 0.02f, 0.25f, 0.08f, 1.0f },
            phongShader_.get(),
            noshine
        );
        selectedNode_ = nullptr;
        std::printf("Inserted tree at [%f, %f, %f]\n",
            lastClickWorldPos_.x, lastClickWorldPos_.y, lastClickWorldPos_.z);
    }

    if (st.movemode && beforeNode_) {
        if (auto* d = beforeNode_->drawable()) {
            d->setModel(glm::translate(glm::mat4(1.0f), lastClickWorldPos_));
            std::printf("Moved selected node to [%f, %f, %f]\n",
                lastClickWorldPos_.x, lastClickWorldPos_.y, lastClickWorldPos_.z);

            selectedNode_ = nullptr;
            beforeNode_ = nullptr;
        }
    }
    if (st.beziermode && st.bezierreset && scene5_)
    {
        scene5_->resetBezier();
        std::printf("Bezier points cleared.\n");
    }
}

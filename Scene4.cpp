// Scene4.cpp
#include "Scene4.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "Draweble.h"
#include "Transform.h"
#include "TextureLoader.h"   

void Scene4::init(Scene& scene,
    const Mesh& meshSphere,
    ShaderProgram* phong)
{
    scene_ = &scene;
    phongShader_ = phong;
    meshSphere_ = &meshSphere;

    auto root = SceneNode::makeGroup();

    lightsScene4_.clear();
    lightsScene4_.push_back(
        Light::makePoint({ 0,0,0 }, { 1,1,1 }, 1.4f, { 1,0,0 })
    );

    SceneNode* sunNode = scene_->addDrawableObject(
        root.get(), *meshSphere_,
        { 0,0,0 }, { 5.17f, 5.17f, 5.17f },
        { 1,1,0.5f,1 },
        phongShader_
    );
    sunNode_ = sunNode;

    GLuint texSun = TextureLoader::loadTexture2D("Models/sun.jpg");
    if (sunNode && sunNode->drawable()) {
        sunNode->drawable()->setTexture(texSun);
        sunNode->drawable()->setShadeModel(0);
    }

    auto zemOrbit = SceneNode::makeGroup();
    zemOrbit_ = zemOrbit.get();

    auto earthPivot = SceneNode::makeGroup();
    earthPivot->setLocal(glm::translate(glm::mat4(1.0f),
        glm::vec3(15.0f, 0, 0)));

    auto earthSpin = SceneNode::makeGroup();
    earthSpin_ = earthSpin.get();

    // Země
    SceneNode* earthNode = scene_->addDrawableObject(
        earthSpin.get(),
        *meshSphere_,
        { 0,0,0 }, { 1,1,1 },
        { 1,1,1,1 },
        phongShader_
    );
    GLuint texEarth = TextureLoader::loadTexture2D("Models/zeme.jpg");
    if (earthNode && earthNode->drawable()) {
        earthNode->drawable()->setTexture(texEarth);
        earthNode->drawable()->setShadeModel(1);
    }
    earthPivot->addChild(std::move(earthSpin));

    // Měsíc – orbit kolem Země
    auto moonOrbit = SceneNode::makeGroup();
    moonOrbit_ = moonOrbit.get();

    auto moonPivot = SceneNode::makeGroup();
    moonPivot->setLocal(glm::translate(glm::mat4(1.0f),
        glm::vec3(2.0f, 0.0f, 0.0f)));

    auto moonSpin = SceneNode::makeGroup();
    moonSpin_ = moonSpin.get();

    SceneNode* moonNode = scene_->addDrawableObject(
        moonSpin.get(),
        *meshSphere_,
        { 0,0,0 },
        { 0.63f, 0.63f, 0.63f },
        { 1,1,1,1 },
        phongShader_
    );
    GLuint texMoon = TextureLoader::loadTexture2D("Models/moon.jpg");
    if (moonNode && moonNode->drawable()) {
        moonNode->drawable()->setTexture(texMoon);
        moonNode->drawable()->setShadeModel(1);
    }

    moonPivot->addChild(std::move(moonSpin));
    moonOrbit->addChild(std::move(moonPivot));
    earthPivot->addChild(std::move(moonOrbit));

    zemOrbit->addChild(std::move(earthPivot));
    root->addChild(std::move(zemOrbit));


    auto merOrbit = SceneNode::makeGroup();
    merOrbit_ = merOrbit.get();

    auto merPivot = SceneNode::makeGroup();
    merPivot->setLocal(glm::translate(glm::mat4(1.0f),
        glm::vec3(0, 0, 7.0f)));

    auto merSpin = SceneNode::makeGroup();
    merSpin_ = merSpin.get();

    SceneNode* merNode = scene_->addDrawableObject(
        merSpin.get(), *meshSphere_,
        { 0,0,0 }, { 0.71f, 0.71f, 0.71f },
        { 1,1,1,1 },
        phongShader_
    );
    GLuint texMer = TextureLoader::loadTexture2D("Models/mercury.jpg");
    if (merNode && merNode->drawable()) {
        merNode->drawable()->setTexture(texMer);
        merNode->drawable()->setShadeModel(1);
    }
    merPivot->addChild(std::move(merSpin));
    merOrbit->addChild(std::move(merPivot));
    root->addChild(std::move(merOrbit));


    auto venOrbit = SceneNode::makeGroup();
    venOrbit_ = venOrbit.get();

    auto venPivot = SceneNode::makeGroup();
    venPivot->setLocal(glm::translate(glm::mat4(1.0f),
        glm::vec3(11.0f, 0, 0)));

    auto venSpin = SceneNode::makeGroup();
    venSpin_ = venSpin.get();

    SceneNode* venNode = scene_->addDrawableObject(
        venSpin.get(), *meshSphere_,
        { 0,0,0 }, { 0.98f, 0.98f, 0.98f },
        { 1,1,1,1 },
        phongShader_
    );
    GLuint texVen = TextureLoader::loadTexture2D("Models/venus.jpg");
    if (venNode && venNode->drawable()) {
        venNode->drawable()->setTexture(texVen);
        venNode->drawable()->setShadeModel(1);
    }
    venPivot->addChild(std::move(venSpin));
    venOrbit->addChild(std::move(venPivot));
    root->addChild(std::move(venOrbit));


    auto marOrbit = SceneNode::makeGroup();
    marOrbit_ = marOrbit.get();

    auto marPivot = SceneNode::makeGroup();
    marPivot->setLocal(glm::translate(glm::mat4(1.0f),
        glm::vec3(19.0f, 0, 0)));

    auto marSpin = SceneNode::makeGroup();
    marSpin_ = marSpin.get();

    SceneNode* marNode = scene_->addDrawableObject(
        marSpin.get(), *meshSphere_,
        { 0,0,0 }, { 0.80f, 0.80f, 0.80f },
        { 1,1,1,1 },
        phongShader_
    );
    GLuint texMar = TextureLoader::loadTexture2D("Models/mars.jpg");
    if (marNode && marNode->drawable()) {
        marNode->drawable()->setTexture(texMar);
        marNode->drawable()->setShadeModel(1);
    }
    marPivot->addChild(std::move(marSpin));
    marOrbit->addChild(std::move(marPivot));
    root->addChild(std::move(marOrbit));


    auto jupOrbit = SceneNode::makeGroup();
    jupOrbit_ = jupOrbit.get();

    auto jupPivot = SceneNode::makeGroup();
    jupPivot->setLocal(glm::translate(glm::mat4(1.0f),
        glm::vec3(27.0f, 0, 0)));

    auto jupSpin = SceneNode::makeGroup();
    jupSpin_ = jupSpin.get();

    SceneNode* jupNode = scene_->addDrawableObject(
        jupSpin.get(), *meshSphere_,
        { 0,0,0 }, { 2.33f, 2.33f, 2.33f },
        { 1,1,1,1 },
        phongShader_
    );
    GLuint texJup = TextureLoader::loadTexture2D("Models/jupiter.jpg");
    if (jupNode && jupNode->drawable()) {
        jupNode->drawable()->setTexture(texJup);
        jupNode->drawable()->setShadeModel(1);
    }
    jupPivot->addChild(std::move(jupSpin));
    jupOrbit->addChild(std::move(jupPivot));
    root->addChild(std::move(jupOrbit));


    auto satOrbit = SceneNode::makeGroup();
    satOrbit_ = satOrbit.get();

    auto satPivot = SceneNode::makeGroup();
    satPivot->setLocal(glm::translate(glm::mat4(1.0f),
        glm::vec3(35.0f, 0, 0)));

    auto satSpin = SceneNode::makeGroup();
    satSpin_ = satSpin.get();

    SceneNode* satNode = scene_->addDrawableObject(
        satSpin.get(), *meshSphere_,
        { 0,0,0 }, { 2.19f, 2.19f, 2.19f },
        { 1,1,1,1 },
        phongShader_
    );
    GLuint texSat = TextureLoader::loadTexture2D("Models/saturn.jpg");
    if (satNode && satNode->drawable()) {
        satNode->drawable()->setTexture(texSat);
        satNode->drawable()->setShadeModel(1);
    }
    satPivot->addChild(std::move(satSpin));
    satOrbit->addChild(std::move(satPivot));
    root->addChild(std::move(satOrbit));


    auto uraOrbit = SceneNode::makeGroup();
    uraOrbit_ = uraOrbit.get();

    auto uraPivot = SceneNode::makeGroup();
    uraPivot->setLocal(glm::translate(glm::mat4(1.0f),
        glm::vec3(43.0f, 0, 0)));

    auto uraSpin = SceneNode::makeGroup();
    uraSpin_ = uraSpin.get();

    SceneNode* uraNode = scene_->addDrawableObject(
        uraSpin.get(), *meshSphere_,
        { 0,0,0 }, { 1.63f, 1.63f, 1.63f },
        { 1,1,1,1 },
        phongShader_
    );
    GLuint texUra = TextureLoader::loadTexture2D("Models/uranus.jpg");
    if (uraNode && uraNode->drawable()) {
        uraNode->drawable()->setTexture(texUra);
        uraNode->drawable()->setShadeModel(1);
    }
    uraPivot->addChild(std::move(uraSpin));
    uraOrbit->addChild(std::move(uraPivot));
    root->addChild(std::move(uraOrbit));


    auto nepOrbit = SceneNode::makeGroup();
    nepOrbit_ = nepOrbit.get();

    auto nepPivot = SceneNode::makeGroup();
    nepPivot->setLocal(glm::translate(glm::mat4(1.0f),
        glm::vec3(51.0f, 0, 0)));

    auto nepSpin = SceneNode::makeGroup();
    nepSpin_ = nepSpin.get();

    SceneNode* nepNode = scene_->addDrawableObject(
        nepSpin.get(), *meshSphere_,
        { 0,0,0 }, { 1.61f, 1.61f, 1.61f },
        { 1,1,1,1 },
        phongShader_
    );
    GLuint texNep = TextureLoader::loadTexture2D("Models/neptune.jpg");
    if (nepNode && nepNode->drawable()) {
        nepNode->drawable()->setTexture(texNep);
        nepNode->drawable()->setShadeModel(1);
    }
    nepPivot->addChild(std::move(nepSpin));
    nepOrbit->addChild(std::move(nepPivot));
    root->addChild(std::move(nepOrbit));

    scene_->addRoot(std::move(root));
}

void Scene4::update(float dt)
{
    time_ += dt;

    if (zemOrbit_) {
        glm::mat4 rotOrbit = glm::rotate(glm::mat4(1.0f),
            time_ * 0.25f,
            glm::vec3(0, 1, 0));
        zemOrbit_->setLocal(rotOrbit);
    }

    if (moonOrbit_) {
        glm::mat4 rotMoonOrbit = glm::rotate(glm::mat4(1.0f),
            time_ * 1.5f,
            glm::vec3(0, 1, 0));
        moonOrbit_->setLocal(rotMoonOrbit);
    }

    // Rotace Země kolem osy
    if (earthSpin_) {
        glm::mat4 rotEarth = glm::rotate(glm::mat4(1.0f),
            time_ * 1.0f,
            glm::vec3(0, 1, 0));
        earthSpin_->setLocal(rotEarth);
    }

    // Rotace Měsíce kolem osy
    if (moonSpin_) {
        glm::mat4 rotMoon = glm::rotate(glm::mat4(1.0f),
            time_ * 0.8f,
            glm::vec3(0, 1, 0));
        moonSpin_->setLocal(rotMoon);
    }

    // Merkur – oběh + rotace
    if (merOrbit_) {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f),
            time_ * 4.7f,
            glm::vec3(0, 1, 0));
        merOrbit_->setLocal(rot);
    }
    if (merSpin_) {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f),
            time_ * 0.02f,
            glm::vec3(0, 1, 0));
        merSpin_->setLocal(rot);
    }

    // Venuše
    if (venOrbit_) {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f),
            time_ * 1.2f,
            glm::vec3(0, 1, 0));
        venOrbit_->setLocal(rot);
    }
    if (venSpin_) {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f),
            time_ * -0.004f,
            glm::vec3(0, 1, 0));
        venSpin_->setLocal(rot);
    }

    // Mars
    if (marOrbit_) {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f),
            time_ * 0.53f,
            glm::vec3(0, 1, 0));
        marOrbit_->setLocal(rot);
    }
    if (marSpin_) {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f),
            time_ * 0.97f,
            glm::vec3(0, 1, 0));
        marSpin_->setLocal(rot);
    }

    // Jupiter
    if (jupOrbit_) {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f),
            time_ * 0.083f,
            glm::vec3(0, 1, 0));
        jupOrbit_->setLocal(rot);
    }
    if (jupSpin_) {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f),
            time_ * 2.4f,
            glm::vec3(0, 1, 0));
        jupSpin_->setLocal(rot);
    }

    // Saturn
    if (satOrbit_) {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f),
            time_ * 0.034f,
            glm::vec3(0, 1, 0));
        satOrbit_->setLocal(rot);
    }
    if (satSpin_) {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f),
            time_ * 2.3f,
            glm::vec3(0, 1, 0));
        satSpin_->setLocal(rot);
    }

    // Uran
    if (uraOrbit_) {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f),
            time_ * 0.012f,
            glm::vec3(0, 1, 0));
        uraOrbit_->setLocal(rot);
    }
    if (uraSpin_) {
        glm::mat4 tilt = glm::rotate(glm::mat4(1.0f),
            glm::radians(82.0f),
            glm::vec3(1, 0, 0));
        glm::mat4 spin = glm::rotate(glm::mat4(1.0f),
            time_ * -1.4f,
            glm::vec3(0, 1, 0));
        uraSpin_->setLocal(tilt * spin);
    }

    // Neptun
    if (nepOrbit_) {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f),
            time_ * 0.006f,
            glm::vec3(0, 1, 0));
        nepOrbit_->setLocal(rot);
    }
    if (nepSpin_) {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f),
            time_ * 1.5f,
            glm::vec3(0, 1, 0));
        nepSpin_->setLocal(rot);
    }
}

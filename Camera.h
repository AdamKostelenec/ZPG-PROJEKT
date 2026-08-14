#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    explicit Camera(const glm::vec3& position = { 0.0f, 0.0f, 0.0f },
        float yawDeg = -90.0f, float pitchDeg = 0.0f,
        float fovDeg = 50.0f, float zNear = 0.1f, float zFar = 100.0f); //upravy v controlleru

    glm::mat4 view() const;
    glm::mat4 proj(float aspect) const;

    // Ovládání
    void setPosition(const glm::vec3& p);
    void lookAt(const glm::vec3& target);              
    void processKeyboard(int keyW, int keyA, int keyS, int keyD, float deltaTime);
    void processMouse(float dx, float dy, bool invertX = false, bool invertY = false);

    // Gettery
    const glm::vec3& position() const { return pos_; }
    const glm::vec3& front() const { return front_; }
    float yaw()   const { return yawDeg_; }
    float pitch() const { return pitchDeg_; }
    float fov()   const { return fovDeg_; }

    // Nastaveni citlivosti/rychlosti
    void setSpeed(float v) { moveSpeed_ = v; }
    void setSensitivity(float s) { mouseSensitivity_ = s; }

    //move
    void setFreeMove(bool b) { freeMove_ = b; }
    bool isFreeMove() const { return freeMove_; }
   

private:
    glm::vec3 pos_{ 0,0,0 };
    glm::vec3 front_{ 0,0,0 };
    glm::vec3 up_{ 0,0,0 };
    glm::vec3 right_{ 0,0,0 };
    float yawDeg_ = -90.0f;   // smer -Z
    float pitchDeg_ = 0.0f;
    float fovDeg_ = 50.0f;
    float zNear_ = 0.1f;
    float zFar_ = 100.0f;

    float moveSpeed_ = 3.0f;   // jednotek / s
    float mouseSensitivity_ = 0.12f;  // stupne na pixel

    void updateVectors_();  
    bool freeMove_ = false;// z yaw/pitch spocita front/right/up
};

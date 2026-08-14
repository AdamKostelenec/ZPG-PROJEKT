#include "Camera.h"
#include <algorithm>
#include <cmath>

static inline float rad(float deg) { return deg * 3.14159265358979323846f / 180.0f; }

Camera::Camera(const glm::vec3& position, float yawDeg, float pitchDeg, float fovDeg, float zNear, float zFar)
    : pos_(position), yawDeg_(yawDeg), pitchDeg_(pitchDeg), fovDeg_(fovDeg), zNear_(zNear), zFar_(zFar) {
    updateVectors_();
}

glm::mat4 Camera::view() const {
    return glm::lookAt(pos_, pos_ + front_, up_);
}

glm::mat4 Camera::proj(float aspect) const {
    return glm::perspective(glm::radians(fovDeg_), aspect, zNear_, zFar_);
}

void Camera::setPosition(const glm::vec3& p) {
    pos_ = p;
}

/*void Camera::lookAt(const glm::vec3& target) {//nasteveni orientace kamery
    glm::vec3 dir = glm::normalize(target - pos_);
    pitchDeg_ = std::asin(dir.y) * 180.0f / 3.14159265358979323846f;
    yawDeg_ = std::atan2(dir.z, dir.x) * 180.0f / 3.14159265358979323846f;
    yawDeg_ = 90.0f - yawDeg_; 
    updateVectors_();
}*/

void Camera::processKeyboard(int keyW, int keyA, int keyS, int keyD, float dt) {
    float v = moveSpeed_ * dt;

    glm::vec3 forward = front_;
    glm::vec3 rightVec = right_;

    if (!freeMove_) {
        forward.y = 0.0f;
        rightVec.y = 0.0f;

        forward = glm::normalize(forward);
        rightVec = glm::normalize(rightVec);
		pos_.y = 0.5f; 
    }

    if (keyW) pos_ += forward * v;
    if (keyS) pos_ -= forward * v;
    if (keyD) pos_ += rightVec * v;
    if (keyA) pos_ -= rightVec * v;
}
void Camera::processMouse(float dx, float dy, bool invertX, bool invertY) {
    float sx = invertX ? -1.0f : 1.0f;
    float sy = invertY ? -1.0f : 1.0f;
    yawDeg_ += sx * dx * mouseSensitivity_;
    pitchDeg_ += sy * dy * mouseSensitivity_;

    pitchDeg_ = std::clamp(pitchDeg_, -89.0f, 89.0f);
    if (yawDeg_ > 360.0f) yawDeg_ -= 360.0f;
    if (yawDeg_ < -360.0f) yawDeg_ += 360.0f;

    updateVectors_();
}


void Camera::updateVectors_() {
    // smer z yaw/pitch (OpenGL konvence)
    float cy = std::cos(rad(yawDeg_));
    float sy = std::sin(rad(yawDeg_));
    float cp = std::cos(rad(pitchDeg_));
    float sp = std::sin(rad(pitchDeg_));

    glm::vec3 f;
    f.x = cy * cp;
    f.y = sp;
    f.z = sy * cp * -1.0f; 
    front_ = glm::normalize(f);

    // right a up
    right_ = glm::normalize(glm::cross(front_, glm::vec3(0, 1, 0)));
    up_ = glm::normalize(glm::cross(right_, front_));
}

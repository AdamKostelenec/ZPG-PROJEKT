#pragma once
#include <vector>
#include <glm/glm.hpp>

struct CameraUpdate {
    glm::mat4 view{};
    glm::mat4 proj{};
    glm::vec3 camPos{ 0.0f };
    float     time{ 0.0f };
};

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onCameraUpdate(const CameraUpdate& u) = 0;
};

class CameraSubject {
public:
    void addObserver(IObserver* o);
    void removeObserver(IObserver* o);
    void clearObservers();
    void notifyObservers(const CameraUpdate& u) const;
private:
    std::vector<IObserver*> observers_;
};

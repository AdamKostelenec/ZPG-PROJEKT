#include "Observer.h"
#include <algorithm>

void CameraSubject::addObserver(IObserver* o) {
    if (!o) return;
    if (std::find(observers_.begin(), observers_.end(), o) == observers_.end())
        observers_.push_back(o);
}
void CameraSubject::removeObserver(IObserver* o) {
    if (!o) return;
    observers_.erase(std::remove(observers_.begin(), observers_.end(), o), observers_.end());
}
void CameraSubject::clearObservers() { observers_.clear(); }

void CameraSubject::notifyObservers(const CameraUpdate& u) const {
    for (auto* o : observers_) if (o) o->onCameraUpdate(u);
}

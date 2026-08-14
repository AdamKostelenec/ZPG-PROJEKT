#pragma once
#include <array>
#include <glm/glm.hpp>
#include <vector>
#include "Draweble.h"
#include "Mesh.h"

Drawable makeTriangle(const std::array<float, 3>& A,
    const std::array<float, 3>& B,
    const std::array<float, 3>& C,
    const std::array<float, 4>& color);

Drawable makeRectangle(float x, float y, float w, float h,
    const std::array<float, 4>& color);

inline Drawable makeSquare(float cx, float cy, float s,
    const std::array<float, 4>& color) {
    float h = s * 0.5f;
    return makeRectangle(cx - h, cy - h, s, s, color);
}

Drawable makeCircle(float cx, float cy, float r, int segments,
    const std::array<float, 4>& color);


Mesh makePlaneMeshPN(const glm::vec3& center, const glm::vec2& size);
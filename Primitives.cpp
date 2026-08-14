#include "Primitives.h"
#include <vector>
#include <cmath>

Drawable makeTriangle(const std::array<float, 3>& A,
    const std::array<float, 3>& B,
    const std::array<float, 3>& C,
    const std::array<float, 4>& color)
{
    std::vector<float> v = {
        A[0],A[1],A[2], B[0],B[1],B[2], C[0],C[1],C[2]
    };
    return Drawable{ Mesh(v, {}, GL_TRIANGLES), color };
}


Drawable makeRectangle(float x, float y, float w, float h,
    const std::array<float, 4>& color)
{
    std::vector<float> v = {
        x,   y,   0,  x + w, y,   0,  x + w, y + h, 0,  x,   y + h, 0
    };
    std::vector<unsigned> i = { 0,1,2, 0,2,3 };
    return Drawable{ Mesh(v, i, GL_TRIANGLES), color };
}

Drawable makeCircle(float cx, float cy, float r, int segments,
    const std::array<float, 4>& color)
{
    std::vector<float> v; v.reserve((segments + 2) * 3);
    v.push_back(cx); v.push_back(cy); v.push_back(0.f);
    for (int i = 0; i <= segments; i++) {
        float t = (float)i / segments * 6.28318530718f; // 2π
        v.push_back(cx + r * std::cos(t));
        v.push_back(cy + r * std::sin(t));
        v.push_back(0.f);
    }
    return Drawable{ Mesh(v, {}, GL_TRIANGLE_FAN), color };
}

Mesh makePlaneMeshPN(const glm::vec3& center, const glm::vec2& size)
{
    const float hx = size.x * 0.5f;
    const float hz = size.y * 0.5f;
    const glm::vec3 n(0, 1, 0);

    const float uvScale = 10.0f;  

    std::vector<float> v = {
        center.x - hx, center.y, center.z - hz,  n.x, n.y, n.z,   0.0f,       0.0f,
        center.x + hx, center.y, center.z - hz,  n.x, n.y, n.z,   uvScale,    0.0f,
        center.x + hx, center.y, center.z + hz,  n.x, n.y, n.z,   uvScale,    uvScale,
        center.x - hx, center.y, center.z + hz,  n.x, n.y, n.z,   0.0f,       uvScale
    };

    std::vector<unsigned> idx = { 0, 1, 2, 0, 2, 3 };

    return Mesh(v, /*stride=*/8, idx, GL_TRIANGLES);
}

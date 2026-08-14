#pragma once
#include <glm/glm.hpp>


namespace transform {


    inline glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t)
    {
        return (1.0f - t) * a + t * b; 
    }

    glm::vec3 bezier3(
        const glm::vec3& P0,
        const glm::vec3& P1,
        const glm::vec3& P2,
        const glm::vec3& P3,
        float t);


    glm::vec3 bezier3Tangent(
        const glm::vec3& P0,
        const glm::vec3& P1,
        const glm::vec3& P2,
        const glm::vec3& P3,
        float t);


    glm::vec3 pathPos(const std::vector<glm::vec3>& ctrl, float t);

    glm::vec3 pathTangent(const std::vector<glm::vec3>& ctrl, float t);


    void translate(glm::mat4& m, const glm::vec3& t);
    void rotate(glm::mat4& m, float radians, const glm::vec3& axis);
    void scale(glm::mat4& m, const glm::vec3& s);

} 

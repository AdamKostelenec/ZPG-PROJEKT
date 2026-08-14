#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace transform {

    glm::vec3 bezier3(
        const glm::vec3& P0,
        const glm::vec3& P1,
        const glm::vec3& P2,
        const glm::vec3& P3,
        float t)
    {
		glm::vec3 L0 = lerp(P0, P1, t);
        glm::vec3 L1 = lerp(P1, P2, t);
        glm::vec3 L2 = lerp(P2, P3, t);

        glm::vec3 Q0 = lerp(L0, L1, t);//interpolace mezi vysledky
        glm::vec3 Q1 = lerp(L1, L2, t);

		return lerp(Q0, Q1, t);
    }


    glm::vec3 bezier3Tangent(
        const glm::vec3& P0,
        const glm::vec3& P1,
        const glm::vec3& P2,
        const glm::vec3& P3,
        float t)
    {
        glm::vec3 L0 = lerp(P0, P1, t);
        glm::vec3 L1 = lerp(P1, P2, t);
        glm::vec3 L2 = lerp(P2, P3, t);

        glm::vec3 Q0 = lerp(L0, L1, t);
        glm::vec3 Q1 = lerp(L1, L2, t);

        glm::vec3 tangent = Q1 - Q0;
        if (glm::length(tangent) < 1e-6f)
            return glm::vec3(0.0f, 0.0f, 1.0f); 

        return glm::normalize(tangent);
    }


    glm::vec3 pathPos(const std::vector<glm::vec3>& ctrl, float t)
    {
        if (ctrl.size() < 4) {
            return ctrl.empty() ? glm::vec3(0.0f) : ctrl[0];
        }

        const int segCount = static_cast<int>((ctrl.size() - 1) / 3);
        if (segCount <= 0) {
            return ctrl[0];
        }

        t = glm::clamp(t, 0.0f, 0.9999f);

        float segT = t * segCount;               
        int segIdx = static_cast<int>(segT);     
        float u = segT - static_cast<float>(segIdx);

        if (segIdx < 0) segIdx = 0;
        if (segIdx >= segCount) segIdx = segCount - 1;

        int base = segIdx * 3;
        const glm::vec3& P0 = ctrl[base + 0];
        const glm::vec3& P1 = ctrl[base + 1];
        const glm::vec3& P2 = ctrl[base + 2];
        const glm::vec3& P3 = ctrl[base + 3];

        return bezier3(P0, P1, P2, P3, u);
        bezier3(P0, P1, P2, P3, u);
    }


    glm::vec3 pathTangent(const std::vector<glm::vec3>& ctrl, float t)
    {
        if (ctrl.size() < 4) {
            return glm::vec3(0.0f, 0.0f, 1.0f);
        }

        const int segCount = static_cast<int>((ctrl.size() - 1) / 3);
        if (segCount <= 0) {
            return glm::vec3(0.0f, 0.0f, 1.0f);
        }

        t = glm::clamp(t, 0.0f, 0.9999f);

        float segT = t * segCount;
        int segIdx = static_cast<int>(segT);
        float u = segT - static_cast<float>(segIdx);

        if (segIdx < 0) segIdx = 0;
        if (segIdx >= segCount) segIdx = segCount - 1;

        int base = segIdx * 3;
        const glm::vec3& P0 = ctrl[base + 0];
        const glm::vec3& P1 = ctrl[base + 1];
        const glm::vec3& P2 = ctrl[base + 2];
        const glm::vec3& P3 = ctrl[base + 3];

        glm::vec3 tan = bezier3Tangent(P0, P1, P2, P3, u);
        if (glm::length(tan) < 1e-6f)
            return glm::vec3(0.0f, 0.0f, 1.0f);
        return glm::normalize(tan);
    }



    void translate(glm::mat4& m, const glm::vec3& t)
    {
        m = glm::translate(m, t);
    }

    void rotate(glm::mat4& m, float radians, const glm::vec3& axis)
    {
        m = glm::rotate(m, radians, axis);
    }

    void scale(glm::mat4& m, const glm::vec3& s)
    {
        m = glm::scale(m, s);
    }

} 

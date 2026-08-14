#pragma once
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "ShaderProgram.h"

struct Material {
    float ra = 0.1f;
    float rd = 1.0f;
    float rs = 0.3f;
    float h = 32.0f;  //shinenes
};

class Drawable {
public:

    explicit Drawable(const Mesh& mesh, const std::array<float, 4>& color);

    void setColor(float r, float g, float b, float a = 1.0f);
    void setModel(const glm::mat4& M);
    const glm::mat4& model() const { return model_; }

    void translate(const glm::vec3& t);
    void rotate(float radians, const glm::vec3& axis);
    void scale(const glm::vec3& s);

    //stencil

    void setId(GLuint id) { id_ = id; }
    GLuint id() const { return id_; }

    void setMaterial(const Material& m) { material_ = m; }
    const Material& material() const { return material_; }
	void setTexture(GLuint tex) { texture_ = tex; }

    int shadeModel_ = 2; 
    void setShadeModel(int m) { shadeModel_ = m; }

    void setVisible(bool v) { visible_ = v; }
    bool isVisible() const { return visible_; }
    void drawMVP(const ShaderProgram& prog, const glm::mat4& MVP) const;

    void draw(const ShaderProgram& prog, const glm::mat4& view, const glm::mat4& proj) const;

    void draw(const ShaderProgram& prog) const;

private:
    Mesh       mesh_;
    glm::vec4  color_{ 1,1,1,1 };
    glm::mat4  model_{ 1.0f };
    Material   material_;
	GLuint     texture_ = 0;
    GLuint     id_ = 0;

    bool       visible_ = true;   
};

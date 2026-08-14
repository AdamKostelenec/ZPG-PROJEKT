#include "Draweble.h"
#include <glm/gtc/type_ptr.hpp>
#include "Transform.h"


Drawable::Drawable(const Mesh& mesh, const std::array<float, 4>& color)
    : mesh_(mesh), color_{ color[0], color[1], color[2], color[3] } {
}



void Drawable::setColor(float r, float g, float b, float a) {
    color_ = { r,g,b,a };
}

void Drawable::setModel(const glm::mat4& M) {
    model_ = M;
}

void Drawable::translate(const glm::vec3& t) {
    transform::translate(model_, t);
}

void Drawable::rotate(float radians, const glm::vec3& axis) {
    transform::rotate(model_, radians, axis);
}

void Drawable::scale(const glm::vec3& s) {
    transform::scale(model_, s);
}

void Drawable::drawMVP(const ShaderProgram& prog, const glm::mat4& MVP) const {
    if (!visible_) return;
    prog.use();
    prog.setUniform("uColor", color_);   // glm::vec4
    prog.setUniform("modelMatrix", MVP);
    prog.setUniform("uMat.ra", material_.ra);
    prog.setUniform("uMat.rd", material_.rd);
    prog.setUniform("uMat.rs", material_.rs);
    prog.setUniform("uMat.h", material_.h);
    prog.setUniform("uShadingMode", shadeModel_); 

    

    if (texture_ != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_);
        prog.setUniform("uTex", 0);
        prog.setUniform("uHasTex", true);
    }
    else {
        prog.setUniform("uHasTex", false);
    }
    mesh_.draw();
    ShaderProgram::unuse();
}

void Drawable::draw(const ShaderProgram& prog, const glm::mat4& view, const glm::mat4& proj) const {
    glm::mat4 MVP = proj * view * model_;
    drawMVP(prog, MVP);
}

void Drawable::draw(const ShaderProgram& prog) const {
    static const glm::mat4 I(1.0f);
    drawMVP(prog, I);
}

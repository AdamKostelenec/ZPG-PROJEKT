#include "ShaderProgram.h"
#include <stdexcept>
#include <vector>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram() {
    id_ = glCreateProgram();
}

ShaderProgram::ShaderProgram(const char* vsSrc, const char* fsSrc) : ShaderProgram() {
    Shader vs(GL_VERTEX_SHADER, vsSrc);
    Shader fs(GL_FRAGMENT_SHADER, fsSrc);
    attach(vs);
    attach(fs);
    link();
}

ShaderProgram ShaderProgram::fromFiles(const std::string& vsPath, const std::string& fsPath) {
    ShaderProgram p;
    Shader vs = Shader::fromFile(GL_VERTEX_SHADER, vsPath);
    Shader fs = Shader::fromFile(GL_FRAGMENT_SHADER, fsPath);
    p.attach(vs);
    p.attach(fs);
    p.link();
    return p;
}

ShaderProgram::~ShaderProgram() {
    if (id_) glDeleteProgram(id_);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept { *this = std::move(other); }

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
    if (this == &other) return *this;
    if (id_) glDeleteProgram(id_);
    id_ = other.id_; other.id_ = 0;
    linked_ = other.linked_; other.linked_ = false;
    cache_.clear();
    return *this;
}

void ShaderProgram::attach(const Shader& sh) {
    if (!sh.valid()) throw std::runtime_error("attach: shader není validní");
    glAttachShader(id_, sh.handle());
}

void ShaderProgram::link() {
    glLinkProgram(id_);
    GLint ok = GL_FALSE;
    glGetProgramiv(id_, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLint len = 0;
        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &len);
        std::string log(len, '\0');
        glGetProgramInfoLog(id_, len, nullptr, log.data());
        throw std::runtime_error("Program link error:\n" + log);
    }
    linked_ = true;
    detachAllShaders_(); 
}

void ShaderProgram::use() const { glUseProgram(id_); }
void ShaderProgram::unuse() { glUseProgram(0); }

GLint ShaderProgram::uniformLocation(const char* name) const {
    auto it = cache_.find(name);
    if (it != cache_.end()) return it->second;
    GLint loc = glGetUniformLocation(id_, name);
    cache_[name] = loc;
    return loc;
}

void ShaderProgram::setUniform4f(const char* name, float x, float y, float z, float w) const {
    glUniform4f(uniformLocation(name), x, y, z, w);
}
void ShaderProgram::setUniform1i(const char* name, int v) const {
    glUniform1i(uniformLocation(name), v);
}
void ShaderProgram::setUniformMatrix4fv(const char* name, const float* m, bool transpose) const {
    glUniformMatrix4fv(uniformLocation(name), 1, transpose ? GL_TRUE : GL_FALSE, m);
}

// Skalary
void ShaderProgram::setUniform(const char* name, bool v) const {
    glUniform1i(uniformLocation(name), v ? 1 : 0);
}
void ShaderProgram::setUniform(const char* name, int v) const { 
    glUniform1i(uniformLocation(name), v);
}
void ShaderProgram::setUniform(const char* name, float v) const {
    glUniform1f(uniformLocation(name), v);
}

// Vektory (float)
void ShaderProgram::setUniform(const char* name, const glm::vec2& v) const {
    glUniform2f(uniformLocation(name), v.x, v.y);
}
void ShaderProgram::setUniform(const char* name, const glm::vec3& v) const {
    glUniform3f(uniformLocation(name), v.x, v.y, v.z);
}
void ShaderProgram::setUniform(const char* name, const glm::vec4& v) const {
    glUniform4f(uniformLocation(name), v.x, v.y, v.z, v.w);
}

// Vektory (int)
void ShaderProgram::setUniform(const char* name, const glm::ivec2& v) const {
    glUniform2i(uniformLocation(name), v.x, v.y);
}
void ShaderProgram::setUniform(const char* name, const glm::ivec3& v) const {
    glUniform3i(uniformLocation(name), v.x, v.y, v.z);
}
void ShaderProgram::setUniform(const char* name, const glm::ivec4& v) const {
    glUniform4i(uniformLocation(name), v.x, v.y, v.z, v.w);
}

// Matice
void ShaderProgram::setUniform(const char* name, const glm::mat3& m, bool transpose) const {
    glUniformMatrix3fv(uniformLocation(name), 1, transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(m));
}
void ShaderProgram::setUniform(const char* name, const glm::mat4& m, bool transpose) const {
    glUniformMatrix4fv(uniformLocation(name), 1, transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(m));
}

// Pole uniform (samplery, instancing, kostry)
void ShaderProgram::setUniform1iv(const char* name, const int* data, size_t count) const {
    glUniform1iv(uniformLocation(name), (GLsizei)count, data);
}
void ShaderProgram::setUniform1fv(const char* name, const float* data, size_t count) const {
    glUniform1fv(uniformLocation(name), (GLsizei)count, data);
}
void ShaderProgram::setUniformMat4v(const char* name, const glm::mat4* data, size_t count, bool transpose) const {
    glUniformMatrix4fv(uniformLocation(name), (GLsizei)count, transpose ? GL_TRUE : GL_FALSE,
        reinterpret_cast<const float*>(data));
}

// Varianty podle lokace
void ShaderProgram::setUniform(GLint location, float v) const {
    glUniform1f(location, v);
}
void ShaderProgram::setUniform(GLint location, int v) const {
    glUniform1i(location, v);
}
void ShaderProgram::setUniformMat4(GLint location, const glm::mat4& m, bool transpose) const {
    glUniformMatrix4fv(location, 1, transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(m));
}

// --- ODPOJENÍ SHADERŮ PO LINKU ---
void ShaderProgram::detachAllShaders_() const {
    GLint count = 0;
    glGetProgramiv(id_, GL_ATTACHED_SHADERS, &count);
    if (count <= 0) return;

    std::vector<GLuint> shaders(count);
    GLsizei got = 0;
    glGetAttachedShaders(id_, count, &got, shaders.data());
    for (GLsizei i = 0; i < got; ++i)
        glDetachShader(id_, shaders[i]);
}

void ShaderProgram::onCameraUpdate(const CameraUpdate& u) {
    if (!linked_) return;

    use();  // glUseProgram(id_)

    setUniform(camNames_.uView, u.view);
    setUniform(camNames_.uProj, u.proj);
    if (camNames_.uViewProj) {
        glm::mat4 VP = u.proj * u.view;
        setUniform(camNames_.uViewProj, VP);
    }
    if (camNames_.uCamPos) {
        setUniform(camNames_.uCamPos, u.camPos);
    }

    ShaderProgram::unuse();  // glUseProgram(0)
}

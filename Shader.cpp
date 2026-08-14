#include "Shader.h"
#include <fstream>
#include <sstream>

Shader::Shader(GLenum type, const char* source)
    : id_(compile(type, source)), type_(type) {
}

Shader Shader::fromFile(GLenum type, const std::string& path) {
    std::string src = loadTextFile(path);
    return Shader(type, src.c_str());
}

Shader::~Shader() {
    if (id_) glDeleteShader(id_);
}

Shader::Shader(Shader&& other) noexcept { *this = std::move(other); }
Shader& Shader::operator=(Shader&& other) noexcept {
    if (this == &other) return *this;
    if (id_) glDeleteShader(id_);
    id_ = other.id_;  other.id_ = 0;
    type_ = other.type_; other.type_ = 0;
    return *this;
}

std::string Shader::loadTextFile(const std::string& path) {
    std::ifstream f(path);
    if (!f) throw std::runtime_error("Shader: nelze otevřít soubor: " + path);
    std::ostringstream ss; ss << f.rdbuf();
    return ss.str();
}

GLuint Shader::compile(GLenum type, const char* source) {
    GLuint sh = glCreateShader(type);
    glShaderSource(sh, 1, &source, nullptr);
    glCompileShader(sh);

    GLint ok = GL_FALSE;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        GLint len = 0; glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &len);
        std::string log(len, '\0'); glGetShaderInfoLog(sh, len, nullptr, log.data());
        glDeleteShader(sh);
        throw std::runtime_error("Shader compile error:\n" + log);
    }
    return sh;
}



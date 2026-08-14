#pragma once
#include <GL/glew.h>
#include <string>
#include <stdexcept>

class Shader {
public:
    Shader() = default;                                  
    Shader(GLenum type, const char* source);             
    static Shader fromFile(GLenum type, const std::string& path);  

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    ~Shader();

    GLuint handle() const { return id_; }
    GLenum type()   const { return type_; }
    bool   valid()  const { return id_ != 0; }

private:
    GLuint id_ = 0;
    GLenum type_ = 0;

    static std::string loadTextFile(const std::string& path);
    static GLuint compile(GLenum type, const char* source); 
};

#pragma once
#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include "Shader.h"
#include "Observer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram : public IObserver {
public:
    ShaderProgram();                                    
    ShaderProgram(const char* vsSrc, const char* fsSrc); 
    static ShaderProgram fromFiles(const std::string& vsPath,
        const std::string& fsPath); 

    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ~ShaderProgram();

    void attach(const Shader& sh);   
    void link();                     
    void use() const;               
    static void unuse();             

    GLuint handle() const { return id_; } 
    bool   linked() const { return linked_; }

    GLint uniformLocation(const char* name) const;

    void setUniform4f(const char* name, float x, float y, float z, float w) const;
    void setUniform1i(const char* name, int v) const;
    void setUniformMatrix4fv(const char* name, const float* m, bool transpose = false) const;

    void setUniform(const char* name, bool v) const;
    void setUniform(const char* name, int v) const;
    void setUniform(const char* name, float v) const;

    void setUniform(const char* name, const glm::vec2& v) const;
    void setUniform(const char* name, const glm::vec3& v) const;
    void setUniform(const char* name, const glm::vec4& v) const;

    void setUniform(const char* name, const glm::ivec2& v) const;
    void setUniform(const char* name, const glm::ivec3& v) const;
    void setUniform(const char* name, const glm::ivec4& v) const;

    void setUniform(const char* name, const glm::mat3& m, bool transpose = false) const;
    void setUniform(const char* name, const glm::mat4& m, bool transpose = false) const;

    void setUniform1iv(const char* name, const int* data, size_t count) const;
    void setUniform1fv(const char* name, const float* data, size_t count) const;
    void setUniformMat4v(const char* name, const glm::mat4* data, size_t count, bool transpose = false) const;

    void setUniform(GLint location, float v) const;
    void setUniform(GLint location, int   v) const;
    void setUniformMat4(GLint location, const glm::mat4& m, bool transpose = false) const;

    struct UniformNames {
        const char* uView = "uView";
        const char* uProj = "uProj";
        const char* uViewProj = "uViewProj"; 
        const char* uCamPos = "uCamPos"; 
    };

    void setCameraUniformNames(const UniformNames& n) { camNames_ = n; }

    void onCameraUpdate(const CameraUpdate& u) override;


private:
    GLuint id_ = 0;
    bool linked_ = false;
    mutable std::unordered_map<std::string, GLint> cache_;

    void detachAllShaders_() const; 
    UniformNames camNames_{};
};

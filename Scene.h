#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Draweble.h"
#include "ShaderProgram.h"

class SceneNode {
public:
    explicit SceneNode(const glm::mat4& local = glm::mat4(1.0f));
    static std::unique_ptr<SceneNode> makeGroup(const glm::mat4& local = glm::mat4(1.0f));
    static std::unique_ptr<SceneNode> makeDrawable(Drawable d, const glm::mat4& local = glm::mat4(1.0f));

    void setProgram(ShaderProgram* prog);
    void setLocal(const glm::mat4& m);
    const glm::mat4& local() const;
    void setDrawable(Drawable d);
    Drawable* drawable();
    const Drawable* drawable() const { return drawable_.get(); }
    void addChild(std::unique_ptr<SceneNode> child);
    void draw(const ShaderProgram& defaultProg, const glm::mat4& view,
        const glm::mat4& proj, const glm::mat4& parent) const;
    bool removeChildRecursive(SceneNode* target);


private:
    glm::mat4 local_;
    std::unique_ptr<Drawable> drawable_;
    std::vector<std::unique_ptr<SceneNode>> children_;
    ShaderProgram* program_ = nullptr;
};

class Scene {
public:
    void addRoot(std::unique_ptr<SceneNode> node);
    void draw(const ShaderProgram& prog, const glm::mat4& view, const glm::mat4& proj) const;
    SceneNode* addDrawableObject(SceneNode* parent,
		const Mesh& mesh,
        const glm::vec3& pos,
        const glm::vec3& scale,
        const std::array<float, 4>& color,
        ShaderProgram* prog,
        const Material& mat = Material{});
    
    SceneNode* nodeFromId(GLuint id) {
        if (id == 0 || id > idToNode_.size()) return nullptr;
        return idToNode_[id - 1];
    }
    GLuint registerNode(SceneNode* n) {
        idToNode_.push_back(n);
		return static_cast<GLuint>(idToNode_.size());
    }

    bool deleteNode(SceneNode* target);

private:
    std::vector<std::unique_ptr<SceneNode>> roots_;
    std::vector<SceneNode*> idToNode_;
};

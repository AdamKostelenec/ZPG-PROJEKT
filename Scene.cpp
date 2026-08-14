#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>

// ---- SceneNode ----
SceneNode::SceneNode(const glm::mat4& local) : local_(local) {}

std::unique_ptr<SceneNode> SceneNode::makeGroup(const glm::mat4& local) {
    return std::make_unique<SceneNode>(local);
}

std::unique_ptr<SceneNode> SceneNode::makeDrawable(Drawable d, const glm::mat4& local) {
    auto n = std::make_unique<SceneNode>(local);
    n->drawable_ = std::make_unique<Drawable>(std::move(d));
    return n;
}

void SceneNode::setProgram(ShaderProgram* prog) { program_ = prog; }
void SceneNode::setLocal(const glm::mat4& m) { local_ = m; }
const glm::mat4& SceneNode::local() const { return local_; }
void SceneNode::setDrawable(Drawable d) { drawable_ = std::make_unique<Drawable>(std::move(d)); }
Drawable* SceneNode::drawable() { return drawable_.get(); }
void SceneNode::addChild(std::unique_ptr<SceneNode> child) { children_.push_back(std::move(child)); }

void SceneNode::draw(const ShaderProgram& defaultProg,
    const glm::mat4& view,
    const glm::mat4& proj,
    const glm::mat4& parent) const
{
    const ShaderProgram& prog = program_ ? *program_ : defaultProg;

    glm::mat4 M = parent * local_;

    if (drawable_) {
        const glm::mat4 totalModel = M * drawable_->model();
        const glm::mat4 MVP = proj * view * totalModel;
        prog.use();
        if (prog.uniformLocation("uModel") != -1) {
            prog.setUniform("uModel", totalModel);
        }
        
		glStencilFunc(GL_ALWAYS, drawable_->id(), 0xFF);
        drawable_->drawMVP(prog, MVP);
   
    }

    for (auto& ch : children_)
        ch->draw(prog, view, proj, M);
}

bool SceneNode::removeChildRecursive(SceneNode* target) {
    for (auto it = children_.begin(); it != children_.end(); ++it) {
        if (it->get() == target) {
            children_.erase(it);
            return true;
        }
    }

    for (auto& ch : children_) {
        if (ch->removeChildRecursive(target))
            return true;
    }

    return false; // nenašli jsme
}

// ---- Scene ----
void Scene::addRoot(std::unique_ptr<SceneNode> node) {
    roots_.push_back(std::move(node));
}

void Scene::draw(const ShaderProgram& prog, const glm::mat4& view, const glm::mat4& proj) const {
    const glm::mat4 I(1.0f);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF); // povolíme zápis do stencil bufferu
    for (auto& r : roots_) {
        r->draw(prog, view, proj, I);
    }
	glDisable(GL_STENCIL_TEST);
}


bool Scene::deleteNode(SceneNode* target) {
    if (!target) return false;

    for (auto it = roots_.begin(); it != roots_.end(); ++it) {
        if (it->get() == target) {
            roots_.erase(it);   // smaže celý podstrom
            return true;
        }
    }

    for (auto& r : roots_) {
        if (r->removeChildRecursive(target))
            return true;
    }

    return false; 
}


SceneNode* Scene::addDrawableObject(SceneNode* parent,
    const Mesh& mesh,
    const glm::vec3& pos,
    const glm::vec3& scale,
    const std::array<float, 4>& color,
    ShaderProgram* prog,
    const Material& mat )
{
    Drawable d{ mesh, color };
	d.setMaterial(mat);

    glm::mat4 M(1.f);
    M = glm::translate(M, pos);
    M = glm::scale(M, scale);
    d.setModel(M);

    auto node = SceneNode::makeDrawable(std::move(d));
    node->setProgram(prog);

    SceneNode* raw = node.get();
	GLuint id = registerNode(raw);
    if(auto* dr = raw->drawable())
    {
        dr->setId(id);
	}
    if (parent)
        parent->addChild(std::move(node));
    else
        addRoot(std::move(node));

    return raw;
}

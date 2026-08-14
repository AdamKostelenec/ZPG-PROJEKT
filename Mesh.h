#pragma once
#include <vector>
#include <GL/glew.h>
#include <memory>


class Mesh{

public:

	Mesh() = default;

	Mesh(const std::vector<float>& verts, int stride,
		const std::vector<unsigned>& idx = {},
        GLenum drawMode = GL_TRIANGLES);

    Mesh(const std::vector<float>& verts,
        const std::vector<unsigned>& idx = {},
		GLenum drawMode = GL_TRIANGLES) :Mesh(verts,3,idx,drawMode){}


    Mesh(const Mesh&) = default;
    Mesh& operator=(const Mesh&) = default;
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

	~Mesh();

	void bind() const;
	void unbind() const; 
	void draw() const;

    GLsizei getCount() const { return gpu_ ? gpu_->count : 0; }   
    bool    isIndexed() const { return gpu_ ? gpu_->indexed : false; }
    GLuint  vaoId() const { return gpu_ ? gpu_->vao : 0; }
    int     stride() const { return gpu_ ? gpu_->stride : 3; }




private:

    struct Gpu {                                        
        GLuint  vao = 0, vbo = 0, ebo = 0;
        GLsizei count = 0;
        GLenum  mode = GL_TRIANGLES;
        bool    indexed = false;
        int     stride = 3;
        ~Gpu() {
            if (ebo) glDeleteBuffers(1, &ebo);
            if (vbo) glDeleteBuffers(1, &vbo);
            if (vao) glDeleteVertexArrays(1, &vao);
        }
    };
    void setupVAO(const std::vector<float>& verts, int stride,
        const std::vector<unsigned>& idx);
    void release();

    std::shared_ptr<Gpu> gpu_;

};
Mesh makeMeshWithNormals(const float* data, size_t count, float scale = 1.0f);
Mesh makeMeshWithNormalsUV(const float* data, size_t count, float scale = 1.0f);
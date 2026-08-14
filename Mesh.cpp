#include "Mesh.h"
#include <cmath>
#include <memory> 
#include <cstddef> // size_t, nullptr
#include <vector>

Mesh::Mesh(const std::vector<float>& verts, int stride,
    const std::vector<unsigned>& idx, GLenum m)
{
    gpu_ = std::make_shared<Gpu>();
    gpu_->mode = m;
    gpu_->stride = stride;
    setupVAO(verts, stride, idx);
}

Mesh::~Mesh() { release(); }
void Mesh::release() {
    gpu_.reset();
}

void Mesh::setupVAO(const std::vector<float>& verts, int stride,
    const std::vector<unsigned>& idx)
{
    glGenVertexArrays(1, &gpu_->vao);
    glGenBuffers(1, &gpu_->vbo);

    glBindVertexArray(gpu_->vao);
    glBindBuffer(GL_ARRAY_BUFFER, gpu_->vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

    if (!idx.empty()) {
        gpu_->indexed = true;
        gpu_->count = static_cast<GLsizei>(idx.size());
        glGenBuffers(1, &gpu_->ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu_->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(unsigned), idx.data(), GL_STATIC_DRAW);
    }
    else {
        gpu_->indexed = false;
        gpu_->count = static_cast<GLsizei>(verts.size() / stride);
    }

    const GLsizei s = stride * sizeof(float);

    // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, s, (void*)0);

    // normal
    if (stride >= 6) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, s, (void*)(3 * sizeof(float)));
    }

    if (stride >= 8) {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, s, (void*)(6 * sizeof(float)));
    }

    glBindVertexArray(0);
}

void Mesh::bind()   const { glBindVertexArray(gpu_ ? gpu_->vao : 0); }
void Mesh::unbind() const { glBindVertexArray(0); }

void Mesh::draw() const {
    if (!gpu_ || !gpu_->vao) return;
    glBindVertexArray(gpu_->vao);
    if (gpu_->indexed) glDrawElements(gpu_->mode, gpu_->count, GL_UNSIGNED_INT, nullptr);
    else               glDrawArrays(gpu_->mode, 0, gpu_->count);
    glBindVertexArray(0);
}


Mesh makeMeshWithNormals(const float* arr, size_t count, float scale) {
    std::vector<float> data;
    data.reserve(count);
    for (size_t i = 0; i + 5 < count; i += 6) {
        data.push_back(arr[i + 0] * scale);
        data.push_back(arr[i + 1] * scale);
        data.push_back(arr[i + 2] * scale);
        data.push_back(arr[i + 3]);
        data.push_back(arr[i + 4]);
        data.push_back(arr[i + 5]);
    }
    return Mesh(data, 6);
}
Mesh makeMeshWithNormalsUV(const float* arr, size_t count, float scale) {
    std::vector<float> data;
    data.reserve((count / 6) * 8);

    constexpr float PI = 3.14159265358979323846f;

    for (size_t i = 0; i + 5 < count; i += 6) {
        float px = arr[i + 0];
        float py = arr[i + 1];
        float pz = arr[i + 2];

        float nx = arr[i + 3];
        float ny = arr[i + 4];
        float nz = arr[i + 5];

      
        float len = std::sqrt(nx * nx + ny * ny + nz * nz);
        if (len > 0.00001f) {
            nx /= len; ny /= len; nz /= len;
        }

        float u  = 0.5f + std::atan2(nz, nx) / (2.0f * PI);
        float v = 0.5f + std::asin(ny) / PI;

        data.push_back(px * scale);
        data.push_back(py * scale);
        data.push_back(pz * scale);

        data.push_back(arr[i + 3]);
        data.push_back(arr[i + 4]);
        data.push_back(arr[i + 5]);

        data.push_back(u);
        data.push_back(v);
    }

    return Mesh(data, 8); 
}

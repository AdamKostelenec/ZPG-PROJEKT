#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "ObjLoader.h"

#include <iostream>
#include <vector>
#include <stdexcept>
#include <filesystem>
#include <glm/glm.hpp>

static std::string getBaseDir(const std::string& path) {
    std::filesystem::path p(path);
    if (p.has_parent_path())
        return p.parent_path().string() + "/";
    return std::string();
}

Mesh loadOBJAsMesh(const std::string& filename,
    bool forceNormals,
    float scale,
    std::vector<std::string>* outDiffuseTextures)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::string baseDir = getBaseDir(filename);

    bool ok = tinyobj::LoadObj(
        &attrib, &shapes, &materials, &warn, &err,
        filename.c_str(),
        baseDir.empty() ? nullptr : baseDir.c_str(),
        /*triangulate=*/true,
        /*default_vcols_fallback=*/false
    );

    if (!warn.empty()) std::cerr << "[TinyOBJLoader][WARN] " << warn << "\n";
    if (!ok) {
        std::cerr << "[TinyOBJLoader][ERR] " << err << "\n";
        throw std::runtime_error("loadOBJAsMesh: nepodarilo se nacist " + filename);
    }

    if (outDiffuseTextures) {
        outDiffuseTextures->clear();
        for (const auto& m : materials) {
            if (!m.diffuse_texname.empty()) {
                outDiffuseTextures->push_back(baseDir + m.diffuse_texname);
            }
        }
    }

    std::vector<float> interleaved;
    interleaved.reserve(shapes.size() * 3 * 8); 

    for (const auto& shape : shapes) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
            int fv = shape.mesh.num_face_vertices[f];
            if (fv != 3) {
                index_offset += fv;
                continue;
            }

            glm::vec3 pos[3];
            glm::vec3 nor[3];
            glm::vec2 uv[3];
            bool hasFaceNormals = true;

            for (int v = 0; v < 3; ++v) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                // pozice
                pos[v] = glm::vec3(
                    attrib.vertices[3 * idx.vertex_index + 0] * scale,
                    attrib.vertices[3 * idx.vertex_index + 1] * scale,
                    attrib.vertices[3 * idx.vertex_index + 2] * scale
                );

                // normála
                if (!attrib.normals.empty() && idx.normal_index >= 0) {
                    nor[v] = glm::vec3(
                        attrib.normals[3 * idx.normal_index + 0],
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]
                    );
                }
                else {
                    hasFaceNormals = false;
                }

                // uv
                if (!attrib.texcoords.empty() && idx.texcoord_index >= 0) {
                    float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                    float ty = attrib.texcoords[2 * idx.texcoord_index + 1];
                    uv[v] = glm::vec2(tx, ty);
                }
                else {
                    uv[v] = glm::vec2(0.0f);
                }
            }

            if (forceNormals || !hasFaceNormals) {
                glm::vec3 n = glm::normalize(glm::cross(pos[1] - pos[0], pos[2] - pos[0]));
                nor[0] = nor[1] = nor[2] = n;
            }

            for (int v = 0; v < 3; ++v) {
                interleaved.push_back(pos[v].x);
                interleaved.push_back(pos[v].y);
                interleaved.push_back(pos[v].z);
                interleaved.push_back(nor[v].x);
                interleaved.push_back(nor[v].y);
                interleaved.push_back(nor[v].z);
                interleaved.push_back(uv[v].x);
                interleaved.push_back(uv[v].y);
            }

            index_offset += fv;
        }
    }

    // 8 = P(3) + N(3) + UV(2)
    return Mesh(interleaved, 8, {}, GL_TRIANGLES);
}

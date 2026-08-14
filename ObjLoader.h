#pragma once
#include <string>
#include "Mesh.h"


Mesh loadOBJAsMesh(const std::string& filename,
    bool forceNormals = false,
    float scale = 1.0f,
    std::vector<std::string>* outDiffuseTextures = nullptr);

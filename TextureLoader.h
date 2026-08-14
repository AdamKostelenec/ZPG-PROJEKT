// TextureLoader.h
#pragma once
#include <string>
#include <array>
#include <GL/glew.h>

namespace TextureLoader {

    GLuint loadTexture2D(const std::string& path,
        bool flipVertical = true);

    GLuint loadCubemap(const std::array<std::string, 6>& faces,
        bool flipVertical = false);
}

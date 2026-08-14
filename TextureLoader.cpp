// TextureLoader.cpp
#include "TextureLoader.h"
#include "stb_image.h"
#include <cstdio>

namespace TextureLoader {

    static GLuint createTexture(GLenum target,
        int w, int h, GLenum format,
        const unsigned char* data)
    {
        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(target, tex);

        glTexImage2D(target, 0, format, w, h, 0, format,
            GL_UNSIGNED_BYTE, data);

        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if (target == GL_TEXTURE_CUBE_MAP) {
            glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        }

        glBindTexture(target, 0);
        return tex;
    }

    GLuint loadTexture2D(const std::string& path, bool flipVertical)
    {
        stbi_set_flip_vertically_on_load(flipVertical);

        int w, h, ch;
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 0);

        if (!data) {
            std::fprintf(stderr, "Failed to load texture: %s\n", path.c_str());
            return 0;
        }

        GLenum format = (ch == 4) ? GL_RGBA : GL_RGB;
        GLuint tex = createTexture(GL_TEXTURE_2D, w, h, format, data);

        stbi_image_free(data);
        return tex;
    }

    GLuint loadCubemap(const std::array<std::string, 6>& faces,
        bool flipVertical)
    {
        stbi_set_flip_vertically_on_load(flipVertical);

        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

        int w, h, ch;

        for (GLuint i = 0; i < faces.size(); i++) {
            unsigned char* data = stbi_load(faces[i].c_str(), &w, &h, &ch, 0);
            if (!data) {
                std::printf("Cubemap failed: %s\n", faces[i].c_str());
                continue;
            }
            GLenum format = (ch == 4) ? GL_RGBA : GL_RGB;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, w, h, 0, format,
                GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        return texID;
    }

} // namespace TextureLoader

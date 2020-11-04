//
// Created by karim on 28.10.20.
//

#ifndef VISIX_OPENGL_TEXTURE_H
#define VISIX_OPENGL_TEXTURE_H


#include <nanogui/opengl.h>
#include <memory>
#include <string>

class Texture {
public:
    Texture(int w, int h, const std::vector<float> &textureData) : mTextureId(0) {
        glGenTextures(1, &mTextureId);
        glBindTexture(GL_TEXTURE_2D, mTextureId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED, GL_FLOAT, textureData.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    ~Texture() noexcept {
        if (mTextureId)
            glDeleteTextures(1, &mTextureId);
    }

    void bind() {
        glBindTexture(GL_TEXTURE_2D, mTextureId);
    }

private:
    GLuint mTextureId;
};


#endif //VISIX_OPENGL_TEXTURE_H

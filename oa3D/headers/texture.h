#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <iostream>

#include "oa3D.h"
class Texture {

    private:
        unsigned int texture_id;

    public:

    // constructor for creating a texture
    Texture(const char* texturePath) {

        int width, height, channels;

        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        // Texture wrapping and filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char* image_data = stbi_load(texturePath, &width, &height, &channels, 0);
        if (image_data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
            glGenerateMipmap(GL_TEXTURE_2D);
            std::cout <<  "Loaded Texture: " << texturePath << std::endl;
        } else {
            std::cout <<  "Failed to load Texture: " << texturePath << std::endl;
        }

        // Freeing up resources
        stbi_image_free(image_data);
    }


    unsigned int getId() {
        return texture_id;
    }

};


#endif

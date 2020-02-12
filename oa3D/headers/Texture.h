#ifndef TEXTURE_H
#define TEXTURE_H

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "glad/glad.h"
#include "Image.h"

#include <string>
#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;

// Texture structure
struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};


class TextureUtil
{

    public:

        static unsigned int LoadCubeMap(vector<string> faces)
        {
            unsigned int textureID;
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            int width, height, nrChannels;
            for (unsigned int i = 0; i < faces.size(); i++)
            {
                Image *img = new Image(faces[i].c_str(), &width, &height, &nrChannels, 0);
                unsigned char *data = img->getData();
                if (data)
                {
                    std::cout << "TEXTURE::UTIL::CUBEMAP::LOADED_TEXTURE::  " << faces[i].c_str() << std::endl;
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                }
                else
                {
                    std::cout << "TEXTURE::UTIL::CUBEMAP::FAILED_TEXTURE_LOAD::  " << faces[i].c_str() << std::endl;
                }

                img->free(data);
            }
                glBindTexture(GL_TEXTURE_2D, 0);

            return textureID;

        }

        static unsigned int LoadTexture(std::string path, std::string directory)
        {
            std::string filename = std::string(path);
            filename = directory + '/' + filename;

            unsigned int textureID;
            glGenTextures(1, &textureID);

            int width, height, nrComponents;

            Image *img = new Image(filename.c_str(), &width, &height, &nrComponents, 0);
            unsigned char *data = img->getData();

            if (data)
            {
                GLenum format;

                switch(nrComponents)
                {
                    case 1:
                    format = GL_RED;
                    break;
                    case 3:
                    format = GL_RGB;
                    break;
                    case 4:
                    format = GL_RGBA;
                    break;
                }

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                std::cout << "TEXTURE::UTIL::LOAD_TEXTURE::LOADED::  " << path << std::endl;
            }
            else
            {
                std::cout << "TEXTURE::UTIL::LOAD_TEXTURE::Failed to load at path: " << path << std::endl;
            }

            img->free(data);
            return textureID;
        }


};


#endif

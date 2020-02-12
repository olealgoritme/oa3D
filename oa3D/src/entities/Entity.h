#ifndef _Entities_H
#define _Entities_H

#include <vector>
#include <iostream>
#include "Vertex.h"
#include "glad/glad.h"

#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

using glm::vec3;
using glm::mat4;
using std::vector;


class Entity
{
    public:

        vec3 position;
        vec3 scaling;
        vec3 rotation;
        float rotationAngle;

        Shader shader;

        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        ~Entity() {};
        Entity() {};

        void Draw(Shader &shader)
        {
                this->shader = shader;
                this->shader.Use();
                this->setTransformation(this->position,
                                            this->scaling,
                                            this->rotation,
                                            this->rotationAngle);

                if(!this->vertices.empty())
                    bindData();

                if(!this->textures.empty())
                    bindTextures();

                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
        }

        void setData(vector<Vertex> vertices)
        {
            this->vertices = vertices;
        }

        void setData(vector<Vertex> vertices, vector<unsigned int> indices)
        {
            this->vertices = vertices;
            this->indices = indices;
        }

        void setTransformation(vec3 position, vec3 scaling, vec3 rotation, float rotationAngle)
        {
            this->position = position;
            this->scaling = scaling;
            this->rotation = rotation;
            this->rotationAngle = rotationAngle;
        }

        mat4 getTransformationMatrix()
        {
            vec3 posVector(this->position);
            vec3 scaleVector(this->scaling);
            vec3 rotVector(this->rotation);

            mat4 modelMatrix(1.0); // final matrix
            mat4 translationMatrix = translate(modelMatrix, posVector);
            mat4 rotationMatrix = rotate(modelMatrix, glm::radians(rotationAngle), rotVector);
            mat4 scaleMatrix = scale(modelMatrix, scaleVector);

            mat4 transformedMatrix = translationMatrix * rotationMatrix * scaleMatrix;

            return transformedMatrix;
        }

        void setRotation(float rotx, float roty, float rotz, float angle)
        {
            this->rotation = vec3(rotx, roty, rotz);
            this->rotationAngle = angle;
        }

        void setScaling(float scalex, float scaley, float, float scalez)
        {
            this->scaling = vec3(scalex, scaley, scalez);
        }

        void setPosition(float posx, float posy, float posz)
        {
            this->position = vec3(posx, posy, posz);
        }

        vec3 getPosition()
        {
            return this->position;
        }

        vec3 getRotation()
        {
            return this->rotation;
        }

        float getRotationAngle()
        {
            return this->rotationAngle;
        }

        vec3 getScale()
        {
            return this->scaling;
        }


    private:

        void bindData()
        {
            // create buffers/arrays
            glGenVertexArrays(1, &this->VAO);
            glBindVertexArray(this->VAO);

            glGenBuffers(1, &this->VBO);
            glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
            glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

            if(!this->indices.empty())
            {
                glGenBuffers(1, &this->EBO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);
            }

            // set shader attribute pointers for vertex
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

            // set shader attribute pointers for textures
            if(!this->textures.empty())
            {
                // vertex normals
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

                // vertex texture coords
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

                // vertex tangent
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

                // vertex bitangent
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
            }

            glBindVertexArray(0);

        }

        void bindTextures()
        {
            // bind appropriate textures
            unsigned int diffuseNr  = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr   = 1;
            unsigned int heightNr   = 1;

            for(unsigned int i = 0; i < this->textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                std::string number;
                std::string name = this->textures[i].type;

                if(name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);

                else if(name == "texture_specular")
                    number = std::to_string(specularNr++);

                else if(name == "texture_normal")
                    number = std::to_string(normalNr++);

                else if(name == "texture_height")
                    number = std::to_string(heightNr++);

                // now set the sampler to the correct texture unit
                this->shader.Use();
                this->shader.setInt((name + number).c_str(), i);
                glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
            }
        }
};


#endif

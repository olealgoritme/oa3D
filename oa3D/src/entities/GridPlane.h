#ifndef _GridPlane_H
#define _GridPlane_H

#include "Primitives.h"

// This is a flat square with edges ten units long oriented in the XZ plane of the local coordinate space
class GridPlane : Primitives
{

    public:
        GridPlane() {};
        ~GridPlane() {};

       void Draw(Shader &shader) override
       {
            this->shader = shader;
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 0, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        void setData(int screenWidth, int gridSize)
        {
            this->quadCount = (screenWidth / gridSize);
            this->quadSize  = (2.0f / this->quadCount);

            Vertex vertex;
            for (int x = 0; x < this->quadCount; ++x)
            {
                float xPos = -1.0f + x * quadSize;
                for (int y = 0; y < this->quadCount; ++y)
                {
                    float yPos = -1.0f + y * quadSize;
                    vertex.Position = glm::vec3(xPos, yPos, 0.0f);
                    vertices.push_back(vertex);
                    vertex.Position = glm::vec3(xPos + quadSize, yPos, 0.0f);
                    vertices.push_back(vertex);
                    vertex.Position = glm::vec3(xPos + quadSize, yPos + quadSize, 0.0f);
                    vertices.push_back(vertex);
                    vertex.Position = glm::vec3(xPos, yPos + quadSize, 0.0f);
                    vertices.push_back(vertex);
                }
            }

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // vertex Positions
            glEnableVertexAttribArray(10);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glBindVertexArray(0);
            std::cout << "vertices size: " << this->vertices.size() << std::endl;
        }

    private:
        int quadCount;
        int quadSize;
        unsigned int VAO;
        unsigned int VBO;
        std::vector<Vertex> vertices;

        Shader shader;

};


#endif // _GridPlane_H


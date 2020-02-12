#ifndef _Triangle_H
#define _Triangle_H

#include "Primitives.h"

// 3 vertices form a triangle
class Triangle : Primitives
{

    public:
        Triangle();
       ~Triangle();

       void Draw(Shader &shader) override
       {
            this->shader = shader;

            for(unsigned int i = 0; i < vertices.size(); i++)
            {
                vertices[i].Position.x = 0;
            }
       }

       void setData(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
       {
            this->vertices = vertices;
            this->indices = indices;
       }

    private:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        Shader shader;
};


#endif // _Triangle_H


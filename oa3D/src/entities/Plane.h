#ifndef _Plane_H
#define _Plane_H

#include "Entity.h"

using glm::vec3;
using std::vector;

class Plane : public Entity
{
    public:

        Plane()
        : Entity()
        {

            // Setup data for plane
            float x = 0.5, y = 0.5, z = 0.5;

            // Quad mesh
            Vertex vertex;
            vec3 vec;
            vector<Vertex> vertices;
            vector<unsigned int> indices;

            // Top left
            vec.x = -x;
            vec.y = y;
            vec.z = -z;
            vertex.Position = vec;
            vertices.push_back(vertex);

            // Top right
            vec.x = x;
            vec.y = y;
            vec.z = -z;
            vertex.Position = vec;
            vertices.push_back(vertex);

            // Bottom right
            vec.x = x;
            vec.y = -y;
            vec.z = -z;
            vertex.Position = vec;
            vertices.push_back(vertex);

            // Bottom left
            vec.x = -x;
            vec.y = -y;
            vec.z = -z;
            vertex.Position = vec;
            vertices.push_back(vertex);

            // Triangle index
            //
            // Explanation:
            // * = Vertex Point
            // (x, x) = (Triangle1 index, Triangle2 index)
            //
            // Drawing:
            //
            //   (0,0) *___* (3)
            //         |\  |
            //         | \ |
            //         |  \|
            //    (1)  *___* (2,2)
            //

            // Indices
            indices.push_back(0); // 0  <---
            indices.push_back(1); // 1     | Triangle 1
            indices.push_back(2); // 2  <---

            indices.push_back(0); // 2  <---
            indices.push_back(2); // 3     | Triangle 2
            indices.push_back(3); // 0  <---

            this->setData(vertices, indices);
        }
};


#endif // _Plane_H


#ifndef OA3D_MESH_H
#define OA3D_MESH_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "Shader.h"
#include "Vertex.h"
#include "Texture.h"
#include "BoneMatrix.h"
#include "VertexBoneData.h"

#include "glm/glm.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "assimp/scene.h"

class Mesh
{

    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, std::vector<VertexBoneData> bone_id_weights);
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

        void Draw(Shader &shader);
        void DrawInstanced(Shader &shader);

    private:
        Shader shader;

        /*  Render data  */
        unsigned int VBO_vertices; // standard vertices
        unsigned int VBO_bones;    // bone vertices
        unsigned int EBO;          // indices
        unsigned int VAO;          // array


        void setupMesh();
        void bindBones();
        void bindTextures();

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        std::vector<VertexBoneData> bones_id_weights_for_each_vertex;
};

#endif

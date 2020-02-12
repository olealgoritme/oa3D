#include "Mesh.h"

    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, std::vector<VertexBoneData> bone_id_weights)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->bones_id_weights_for_each_vertex = bone_id_weights;
        setupMesh();
    }

    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        setupMesh();
    }

    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
    {
        this->vertices = vertices;
        this->indices = indices;
        setupMesh();
    }

    // render the mesh
    void Mesh::Draw(Shader &shader)
    {
        this->shader = shader;

        // bind textures
        if(!textures.empty())
            bindTextures();

        // draw mesh
        this->shader.Use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // reset texture
        if(!textures.empty())
            glActiveTexture(GL_TEXTURE0);
    }


    // render instances of mesh
    void Mesh::DrawInstanced(Shader &shader)
    {

        this->shader = shader;

        // bind textures
        if(!textures.empty())
            bindTextures();

        // draw instanced mesh
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, 100);
        glBindVertexArray(0);

        // reset texture
        if(!textures.empty())
            glActiveTexture(GL_TEXTURE0);
    }

    void Mesh::setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO_vertices);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        if(!textures.empty())
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

    void Mesh::bindTextures()
    {
        // bind appropriate textures
        unsigned int diffuseNr  = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr   = 1;
        unsigned int heightNr   = 1;
        unsigned int ambientNr  = 1;
        unsigned int reflectNr  = 1;
        for(unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            std::string name = textures[i].type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);
            else if(name == "texture_normal")
                number = std::to_string(normalNr++);
            else if(name == "texture_height")
                number = std::to_string(heightNr++);
            else if(name == "texture_ambient")
                number = std::to_string(ambientNr++);
            else if(name == "texture_reflection")
                number = std::to_string(reflectNr++);

            // now set the sampler to the correct texture unit
            shader.Use();
            shader.setInt((name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

    }

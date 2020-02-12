#ifndef MODEL_H
#define MODEL_H

#include "glad/glad.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "Shader.h"
#include "Mesh.h"
#include "Image.h"
#include "Texture.h"
#include "Entity.h"

#include <string>
#include <iostream>

using glm::vec3;
using glm::mat4;

class Model : public Entity
{
public:



    Model(std::string const &path)
    : Entity()
    {
        loadModel(path);
    }

    // draws the model with all meshes inside it. uses the attached shader
    void Draw(Shader &shader)
    {
        for(unsigned int i = 0; i < meshes.size(); i++)
        {
            shader.Use();
            meshes[i].Draw(shader);
        }
    }

    void DrawInstanced(Shader &shader)
    {
        for(unsigned int i = 0; i < meshes.size(); i++)
        {
            shader.Use();
            meshes[i].DrawInstanced(shader);
        }
    }


private:
    /*  Model Data */
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures;
    std::vector<Texture> textures_loaded;
    Texture texture;

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const &path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
                                                 aiProcess_Triangulate |
                                                 aiProcess_FlipUVs);

        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::cout << "MODEL::ASSIMP::ERROR " << importer.GetErrorString() << std::endl;
            return;
        }
        else
        {
            std::cout << "MODEL::LOADING::Model: " << path << std::endl;
        }

        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene)
    {

        // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene.
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {

        if(mesh->HasBones()) {
            printf("MODEL::INFO::This mesh has bones\n");
        }

        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // Walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;

            // positions
            if(mesh->mVertices)
            {
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.Position = vector;
            }

            // normals
            if(mesh->mNormals)
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            else vertex.Normal = glm::vec3(0.0f);

            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                // texture coordinates
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else vertex.TexCoords = glm::vec2(0.0f);

            // tangent
            if(mesh->mTangents)
            {
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
            }
            else vertex.Tangent = glm::vec3(0.0f);

            // bitangent
            if(mesh->mTangents)
            {
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else vertex.Bitangent = glm::vec3(0.0f);

            vertices.push_back(vertex);
        }

        // walk through each of the mesh's faces (polygons) to find texture maps (used for lighting)
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // process materials
        if(mesh->mMaterialIndex)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            // 1. diffuse maps
            std::vector<Texture> diffuseMaps = LoadMaterialTextures(this->directory, material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // 2. specular maps
            std::vector<Texture> specularMaps = LoadMaterialTextures(this->directory, material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            // 3. normal maps
            std::vector<Texture> normalMaps = LoadMaterialTextures(this->directory, material, aiTextureType_NORMALS, "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

            // 4. height maps
            std::vector<Texture> heightMaps = LoadMaterialTextures(this->directory, material, aiTextureType_HEIGHT, "texture_height");
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

            // 5. ambient
            std::vector<Texture> ambientMaps = LoadMaterialTextures(this->directory, material, aiTextureType_AMBIENT, "texture_ambient");
            textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());

            // 6. reflection maps
            std::vector<Texture> reflectionMaps = LoadMaterialTextures(this->directory, material, aiTextureType_REFLECTION, "texture_reflection");
            textures.insert(textures.end(), reflectionMaps.begin(), reflectionMaps.end());
        }
        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        std::vector<Texture> LoadMaterialTextures(std::string &directory, aiMaterial *mat, aiTextureType type, std::string typeName)
        {

            std::vector<Texture> textures;

            for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);

                // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
                bool skip = false;
                for(unsigned int j = 0; j < textures_loaded.size(); j++)
                {
                    if(std::strcmp(this->textures_loaded[j].path.data(), str.C_Str()) == 0)
                    {
                        textures.push_back(textures_loaded[j]);
                        skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                        break;
                    }
                }
                if(!skip)
                {   // if texture hasn't been loaded already, load it
                    texture.id = TextureUtil::LoadTexture(str.C_Str(), directory);
                    texture.type = typeName;
                    texture.path = str.C_Str();
                    textures.push_back(texture);
                    this->textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
                }
            }
            return textures;
        }
};
#endif

#ifndef MODEL_H
#define MODEL_H

#include "glad/glad.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "VertexBoneData.h"
#include "BoneMatrix.h"
#include "Entity.h"

#include <string>
#include <iostream>
#include <map>

class AnimatedModel : public Entity
{

public:

    AnimatedModel(std::string const &path)
    : Entity()
    {
        this->loadModel(path);
    }

    // draws the model with all meshes inside it. uses the attached shader
    void Draw(Shader shader)
    {
        if( shader.programId <= 0)
        {
           printf("MODEL::ERROR::SHADER_NOT_CONNECTED!\n");
           return;
        }

        if(hasAnimations)
        {
            for (uint i = 0; i < MAX_BONES; i++) // get location all matrices of bones
            {
            std::string name = "bones[" + std::to_string(i) + "]";// name in shader
                m_bone_location[i] = shader.getUniformLocation(name);
            }

            // bone transformation
            std::vector<aiMatrix4x4> transforms;
            boneTransform((float) ticks_per_second / 1000.0f, transforms);
            for (uint i = 0; i < transforms.size(); i++) // move all matrices for actual model position to shader
            {
                glUniformMatrix4fv(m_bone_location[i], 1, GL_TRUE, (float *)&transforms[i]);
            }
        }

        // draw meshes
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

private:

    bool hasAnimations = false;
    float ticks_per_second = 0.0f;
    const static unsigned int MAX_BONES = 100;

    // model
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;

    const aiScene* scene;
    aiMatrix4x4 m_global_inverse_transform;

    // bones
    std::map<std::string, uint> m_bone_mapping; // maps a bone name and their index
    unsigned int m_num_bones = 0;
    std::vector<BoneMatrix> m_bone_matrices;
    unsigned int m_bone_location[MAX_BONES];

    glm::quat rotate_head_xz = glm::quat();


    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const &path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        scene = importer.ReadFile(path,
                                                 aiProcess_Triangulate |
                                                 aiProcess_FlipUVs |
                                                 aiProcess_CalcTangentSpace |
                                                 aiProcessPreset_TargetRealtime_MaxQuality);

        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::cout << "MODEL::ASSIMP::ERROR " << importer.GetErrorString() << std::endl;
            return;
        }
            std::cout << "Loading Model: " << path << std::endl;

        if (scene->mAnimations[0]->mTicksPerSecond != 0.0)
            ticks_per_second = scene->mAnimations[0]->mTicksPerSecond;
        else
            ticks_per_second = 25.0f;



        std::cout << "scene->HasAnimations() 1: " << scene->HasAnimations() << std::endl;
        std::cout << "scene->mNumMeshes 1: " << scene->mNumMeshes << std::endl;
        std::cout << "scene->mAnimations[0]->mNumChannels 1: " << scene->mAnimations[0]->mNumChannels << std::endl;
        std::cout << "scene->mAnimations[0]->mDuration 1: " << scene->mAnimations[0]->mDuration << std::endl;
        std::cout << "scene->mAnimations[0]->mTicksPerSecond 1: " << scene->mAnimations[0]->mTicksPerSecond << std::endl << std::endl;


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
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        std::vector<VertexBoneData> bones_id_weights_for_each_vertex;

        vertices.reserve(mesh->mNumVertices);
        indices.reserve(mesh->mNumVertices);
        bones_id_weights_for_each_vertex.resize(mesh->mNumVertices);

        // Walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // assimp to glm vec3 conversion

            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            // normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            // texture coordinates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;

            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
            vertices.push_back(vertex);
        }

        // walk through each of the mesh's faces (a face is a mesh, its triangle) and retrieve the corresponding vertex indices (multiple indexes)
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // 1. diffuse maps
        std::vector<Texture> diffuseMaps = TextureUtil::LoadMaterialTextures(this->directory, material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // 2. specular maps
        std::vector<Texture> specularMaps = TextureUtil::LoadMaterialTextures(this->directory, material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        // 3. normal maps
        std::vector<Texture> normalMaps = TextureUtil::LoadMaterialTextures(this->directory, material, aiTextureType_NORMALS, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        // 4. height maps
        std::vector<Texture> heightMaps = TextureUtil::LoadMaterialTextures(this->directory, material, aiTextureType_HEIGHT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());


        // load bones
        for (uint i = 0; i < mesh->mNumBones; i++)
        {
            uint bone_index = 0;
            std::string bone_name(mesh->mBones[i]->mName.data);

            std::cout << mesh->mBones[i]->mName.data << std::endl;

            if (m_bone_mapping.find(bone_name) == m_bone_mapping.end())
            {
                // Allocate an index for a new bone
                bone_index = m_num_bones;
                m_num_bones++;
                BoneMatrix bi;
                m_bone_matrices.push_back(bi);
                m_bone_matrices[bone_index].offset_matrix = mesh->mBones[i]->mOffsetMatrix;
                m_bone_mapping[bone_name] = bone_index;

                std::cout << "bone_name: " << bone_name << "			 bone_index: " << bone_index << std::endl;
            }
            else
            {
                bone_index = m_bone_mapping[bone_name];
            }

            for (uint j = 0; j < mesh->mBones[i]->mNumWeights; j++)
            {
                uint vertex_id = mesh->mBones[i]->mWeights[j].mVertexId;
                float weight = mesh->mBones[i]->mWeights[j].mWeight;
                bones_id_weights_for_each_vertex[vertex_id].addBoneData(bone_index, weight);
            }
        }
        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures, bones_id_weights_for_each_vertex);
    }


unsigned int findPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	for (uint i = 0; i < p_node_anim->mNumPositionKeys - 1; i++)
	{
		if (p_animation_time < (float)p_node_anim->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

unsigned int findRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	for (uint i = 0; i < p_node_anim->mNumRotationKeys - 1; i++)
	{
		if (p_animation_time < (float)p_node_anim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

unsigned int findScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	for (uint i = 0; i < p_node_anim->mNumScalingKeys - 1; i++)
	{
 		if (p_animation_time < (float)p_node_anim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

aiVector3D calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	if (p_node_anim->mNumPositionKeys == 1) // Keys ��� ������� �����
	{
		return p_node_anim->mPositionKeys[0].mValue;
	}

	uint position_index = findPosition(p_animation_time, p_node_anim);
	uint next_position_index = position_index + 1;
	assert(next_position_index < p_node_anim->mNumPositionKeys);
	float delta_time = (float)(p_node_anim->mPositionKeys[next_position_index].mTime - p_node_anim->mPositionKeys[position_index].mTime);
	float factor = (p_animation_time - (float)p_node_anim->mPositionKeys[position_index].mTime) / delta_time;
	assert(factor >= 0.0f && factor <= 1.0f);
	aiVector3D start = p_node_anim->mPositionKeys[position_index].mValue;
	aiVector3D end = p_node_anim->mPositionKeys[next_position_index].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

aiQuaternion calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	if (p_node_anim->mNumRotationKeys == 1)
	{
		return p_node_anim->mRotationKeys[0].mValue;
	}

	uint rotation_index = findRotation(p_animation_time, p_node_anim);
	uint next_rotation_index = rotation_index + 1;
	assert(next_rotation_index < p_node_anim->mNumRotationKeys);
	float delta_time = (float)(p_node_anim->mRotationKeys[next_rotation_index].mTime - p_node_anim->mRotationKeys[rotation_index].mTime);
	float factor = (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) / delta_time;

	assert(factor >= 0.0f && factor <= 1.0f);
	aiQuaternion start_quat = p_node_anim->mRotationKeys[rotation_index].mValue;
	aiQuaternion end_quat = p_node_anim->mRotationKeys[next_rotation_index].mValue;

	return nlerp(start_quat, end_quat, factor);
}

aiVector3D calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	if (p_node_anim->mNumScalingKeys == 1) // Keys ��� ������� �����
	{
		return p_node_anim->mScalingKeys[0].mValue;
	}

	uint scaling_index = findScaling(p_animation_time, p_node_anim);
	uint next_scaling_index = scaling_index + 1;
	assert(next_scaling_index < p_node_anim->mNumScalingKeys);
	float delta_time = (float)(p_node_anim->mScalingKeys[next_scaling_index].mTime - p_node_anim->mScalingKeys[scaling_index].mTime);
	float  factor = (p_animation_time - (float)p_node_anim->mScalingKeys[scaling_index].mTime) / delta_time;
	assert(factor >= 0.0f && factor <= 1.0f);
	aiVector3D start = p_node_anim->mScalingKeys[scaling_index].mValue;
	aiVector3D end = p_node_anim->mScalingKeys[next_scaling_index].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

const aiNodeAnim * findNodeAnim(const aiAnimation * p_animation, const std::string p_node_name)
{
	// channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
	// numChannels == numBones
	for (uint i = 0; i < p_animation->mNumChannels; i++)
	{
		const aiNodeAnim* node_anim = p_animation->mChannels[i];
		if (std::string(node_anim->mNodeName.data) == p_node_name)
		{
			return node_anim;
		}
	}

	return nullptr;
}
								// start from RootNode
void readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform)
{

    std::string node_name(p_node->mName.data);

	const aiAnimation* animation = scene->mAnimations[0];
	aiMatrix4x4 node_transform = p_node->mTransformation;

	const aiNodeAnim* node_anim = findNodeAnim(animation, node_name);

	if (node_anim)
	{

		//scaling
		//aiVector3D scaling_vector = node_anim->mScalingKeys[2].mValue;
		aiVector3D scaling_vector = calcInterpolatedScaling(p_animation_time, node_anim);
		aiMatrix4x4 scaling_matr;
		aiMatrix4x4::Scaling(scaling_vector, scaling_matr);

		//rotation
		//aiQuaternion rotate_quat = node_anim->mRotationKeys[2].mValue;
		aiQuaternion rotate_quat = calcInterpolatedRotation(p_animation_time, node_anim);
		aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

		//translation
		//aiVector3D translate_vector = node_anim->mPositionKeys[2].mValue;
		aiVector3D translate_vector = calcInterpolatedPosition(p_animation_time, node_anim);
		aiMatrix4x4 translate_matr;
		aiMatrix4x4::Translation(translate_vector, translate_matr);

		if ( std::string(node_anim->mNodeName.data) == std::string("Head"))
		{
			aiQuaternion rotate_head = aiQuaternion(rotate_head_xz.w, rotate_head_xz.x, rotate_head_xz.y, rotate_head_xz.z);

			node_transform = translate_matr * (rotate_matr * aiMatrix4x4(rotate_head.GetMatrix())) * scaling_matr;
		}
		else
		{
			node_transform = translate_matr * rotate_matr * scaling_matr;
		}

	}

	aiMatrix4x4 global_transform = parent_transform * node_transform;

	if (m_bone_mapping.find(node_name) != m_bone_mapping.end()) // true if node_name exist in bone_mapping
	{
		uint bone_index = m_bone_mapping[node_name];
		m_bone_matrices[bone_index].final_world_transformation = m_global_inverse_transform * global_transform * m_bone_matrices[bone_index].offset_matrix;
	}

	for (uint i = 0; i < p_node->mNumChildren; i++)
	{
		readNodeHierarchy(p_animation_time, p_node->mChildren[i], global_transform);
	}

}

void boneTransform(double time_in_sec, std::vector<aiMatrix4x4>& transforms)
{
	aiMatrix4x4 identity_matrix; // = mat4(1.0f);

	double time_in_ticks = time_in_sec * ticks_per_second;
	float animation_time = fmod(time_in_ticks, scene->mAnimations[0]->mDuration);

	readNodeHierarchy(animation_time, scene->mRootNode, identity_matrix);

	transforms.resize(m_num_bones);

	for (uint i = 0; i < m_num_bones; i++)
	{
		transforms[i] = m_bone_matrices[i].final_world_transformation;
	}
}

glm::mat4 aiToGlm(aiMatrix4x4 ai_matr)
{
	glm::mat4 result;
	result[0].x = ai_matr.a1; result[0].y = ai_matr.b1; result[0].z = ai_matr.c1; result[0].w = ai_matr.d1;
	result[1].x = ai_matr.a2; result[1].y = ai_matr.b2; result[1].z = ai_matr.c2; result[1].w = ai_matr.d2;
	result[2].x = ai_matr.a3; result[2].y = ai_matr.b3; result[2].z = ai_matr.c3; result[2].w = ai_matr.d3;
	result[3].x = ai_matr.a4; result[3].y = ai_matr.b4; result[3].z = ai_matr.c4; result[3].w = ai_matr.d4;

	return result;
}

aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend)
{
	a.Normalize();
	b.Normalize();

	aiQuaternion result;
	float dot_product = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	float one_minus_blend = 1.0f - blend;

	if (dot_product < 0.0f)
	{
		result.x = a.x * one_minus_blend + blend * -b.x;
		result.y = a.y * one_minus_blend + blend * -b.y;
		result.z = a.z * one_minus_blend + blend * -b.z;
		result.w = a.w * one_minus_blend + blend * -b.w;
	}
	else
	{
		result.x = a.x * one_minus_blend + blend * b.x;
		result.y = a.y * one_minus_blend + blend * b.y;
		result.z = a.z * one_minus_blend + blend * b.z;
		result.w = a.w * one_minus_blend + blend * b.w;
	}

	return result.Normalize();
}


};


#endif

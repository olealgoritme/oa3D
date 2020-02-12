#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "Shader.h"
#include "OA3D.h"

#include <iostream>
#include <vector>

class ShaderManager
{
    private:
        std::vector<OA3D::Scope<Shader>> vShaders;
        Shader activeShader;

    public:
        ShaderManager();
        ~ShaderManager();

        Shader getActiveShader();
        void add(OA3D::Scope<Shader> shader);
        //void remove(OA3D::Scope<Shader> &shader); // dont need remove? as unique_ptrs disappear

};


ShaderManager::ShaderManager()
{
    std::cout << "ShaderManager::INITIALIZED" << std::endl;
}

ShaderManager::~ShaderManager()
{
    std::cout << "ShaderManager::UNINITIALIZED" << std::endl;
}


Shader ShaderManager::getActiveShader()
{
    return this->activeShader;
}

void ShaderManager::add(OA3D::Scope<Shader> shader)
{
   vShaders.push_back(std::move(shader));
}

#endif

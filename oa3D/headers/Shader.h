#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "IShaderCallback.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


//static IShaderCallback *callback = nullptr;

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

class Shader
{

    public:
        GLuint programId; // Shader Program ID
        const char* vertexPath;
        const char* fragmentPath;
        const char* geometryPath;

        Shader() {};
        Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
        ~Shader();

        unsigned int getUniformLocation(const std::string &name) const;

        void setBool(const std::string &name, bool value) const;

        void setInt(const std::string &name, int value) const;

        void setFloat(const std::string &name, float value) const;

        void setVec2(const std::string &name, const glm::vec2 &value) const;
        void setVec2(const std::string &name, float x, float y) const;

        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setVec3(const std::string &name, float x, float y, float z) const;

        void setVec4(const std::string &name, const glm::vec4 &value) const;
        void setVec4(const std::string &name, float x, float y, float z, float w) const;

        void setMat2(const std::string &name, const glm::mat2 &mat) const;
        void setMat3(const std::string &name, const glm::mat3 &mat) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;
        void setMat4All(const GLuint glLoc, const glm::mat4 &mat, GLboolean normalize) const;

        Shader * ReloadShader(Shader *shader);

        //void setCallback(IShaderCallback *_callback) const;
        void Use() const;
        void Delete() const;

    private:

        void checkCompileErrors(GLuint shader, std::string type);
};

#endif

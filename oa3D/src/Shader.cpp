#include "Shader.h"

/*
 *
 * Shader class
 * for reading GLSL type files, loading, compiling and including in GL program
 * and for passing transforms between CPU and GPU
 *
 */

    Shader::Shader(const char* _vertexPath, const char* _fragmentPath, const char* _geometryPath)
    {
        unsigned int vertex   = 0;
        unsigned int fragment = 0;
        unsigned int geometry = 0;

        bool hasVertex   = (_vertexPath   != nullptr) ? true : false;
        bool hasFragment = (_fragmentPath != nullptr) ? true : false;
        bool hasGeometry = (_geometryPath != nullptr) ? true : false;

        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;

        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;

        std::stringstream vShaderStream;
        std::stringstream fShaderStream;
        std::stringstream gShaderStream;

        const char * vShaderCode;
        const char * fShaderCode;
        const char * gShaderCode;

        // Vertex
        if (hasVertex)
        {
            this->vertexPath = _vertexPath;
            std::cout <<  "SHADER::CREATED::Vertex Shader: " << this->vertexPath << std::endl;

            // Read shader file
            vShaderFile.open(this->vertexPath);
            vShaderStream << vShaderFile.rdbuf();
            vShaderFile.close();
            vertexCode = vShaderStream.str();
            vShaderCode = vertexCode.c_str();

            // Compile Vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            checkCompileErrors(vertex, "VERTEX");

        }

        // Fragment
        if (hasFragment)
        {
            this->fragmentPath = _fragmentPath;
            std::cout <<  "SHADER::CREATED::Fragment Shader: " << this->fragmentPath << std::endl;

            // Read file
            fShaderFile.open(this->fragmentPath);
            fShaderStream << fShaderFile.rdbuf();
            fShaderFile.close();
            fragmentCode = fShaderStream.str();
            fShaderCode = fragmentCode.c_str();

            // Create & compile
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            checkCompileErrors(fragment, "FRAGMENT");

        }

        // Geometry
        if (hasGeometry)
        {
            this->geometryPath = _geometryPath;
            std::cout <<  "SHADER::CREATED::Geometry Shader: " << this->geometryPath << std::endl;

            // Read shader file
            gShaderFile.open(this->geometryPath);
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
            gShaderCode = geometryCode.c_str();

            // Create & compile
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }


        // Shader program
        this->programId = glCreateProgram();
        if (hasVertex)   glAttachShader(this->programId, vertex);
        if (hasFragment) glAttachShader(this->programId, fragment);
        if (hasGeometry) glAttachShader(this->programId, geometry);
        glLinkProgram(this->programId);

        printf("SHADER::PROGRAM::ID: %d\n", this->programId);
        checkCompileErrors(this->programId, "PROGRAM");


        // Delete loaded shaders, as they're linked into program
        if (hasVertex)
        {
            glDeleteShader(vertex);
            std::cout <<  "SHADER::LOADED::Vertex Shader" << std::endl;
        }

        if (hasFragment)
        {
            glDeleteShader(fragment);
            std::cout <<  "SHADER::LOADED::Fragment Shader" << std::endl;
        }

        if (hasGeometry)
        {
            glDeleteShader(geometry);
            std::cout <<  "SHADER::LOADED::Geometry Shader" << std::endl;

        }

    }

    Shader::~Shader()
    {
        //printf("SHADER::UNLOADED OBJECT\n");
        //this->Delete();
    }

    // activates shader program
    void Shader::Use() const
    {
        glUseProgram(this->programId);
    }

    // deletes program
    void Shader::Delete() const
    {
        glDeleteProgram(this->programId);
    }

    /*
     *void Shader::setCallback(IShaderCallback *_callback) const
     *{
     *     callback = _callback;
     *}
     */

    Shader * Shader::ReloadShader(Shader *shader)
    {
        const char* vPath = shader->vertexPath;
        const char* fPath = shader->fragmentPath;
        shader->Delete();
        glUseProgram(0);;
        Shader sh = Shader(vPath, fPath);
        // if(callback != NULL) callback->onShaderReload();
        return &sh;
    }

    /* setters for bool / int float / vec2 / vec3 / vec4 / mat3 / mat4 */
    /********************************************************************/

    unsigned int Shader::getUniformLocation(const std::string &name) const
    {
        return glGetUniformLocation(this->programId, name.c_str());
    }

    void Shader::setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(this->programId, name.c_str()), (int)value);
    }

    void Shader::setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(this->programId, name.c_str()), value);
    }

    void Shader::setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(this->programId, name.c_str()), value);
    }

    void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(this->programId, name.c_str()), 1, &value[0]);
    }

    void Shader::setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(this->programId, name.c_str()), x, y);
    }

    void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(this->programId, name.c_str()), 1, &value[0]);
    }

    void Shader::setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(this->programId, name.c_str()), x, y, z);
    }

    void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(this->programId, name.c_str()), 1, &value[0]);
    }

    void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(this->programId, name.c_str()), x, y, z, w);
    }

    void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(this->programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(this->programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(this->programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void Shader::setMat4All(GLuint glLoc, const glm::mat4 &mat, GLboolean normalize) const
    {
        glUniformMatrix4fv(this->programId, glLoc, normalize, glm::value_ptr(mat));
    }

    // checking shader compilation/linking errors.
    void Shader::checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[2048];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

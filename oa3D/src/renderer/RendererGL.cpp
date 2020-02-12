//
// Created by xuw on 17.01.2020.
//
// OpenGL Renderer

#include "RendererGL.h"


// Instance reference
Renderer* Renderer::m_Instance = nullptr;

// Singleton OpenGL Renderer
Renderer* Renderer::getInstance()
{
    if(!m_Instance)
        m_Instance = new Renderer();

    return m_Instance;
}

// Draws the model object
void Renderer::draw(Entity entity, Shader shader)
{
   entity.Draw(shader);
}

// Draw mesh
void Renderer::drawMesh(Mesh mesh, Shader shader) {
    mesh.Draw(shader);
}

// Draw model
void Renderer::drawModel(Model model, Shader shader) {
    model.Draw(shader);
}

void Renderer::getVAO(GLint *vao)
{
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint *) &vao);
}

// Instanced drawing of the model object
void Renderer::drawInstanced(Entity entity, Shader shader)
{
    // TODO: implement instanced entities
   entity.Draw(shader);
}

void Renderer::enableGLOptions()
{
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        //glFrontFace(GL_CCW);
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        //Instanced opengl testing
        //if(INSTANCED) instance_setup();
}

// Simple error clearing in GL
void GLClearErrors()
{
    while(glGetError() != GL_NO_ERROR);
}

bool GLError(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (0x" << std::hex << error << ")\n"
                    << function << "\n"
                    << file << ":" << std::dec << line << std::endl;
        return false;
    }
    return true;
}

//
// Created by xuw on 17.01.2020.
//

#ifndef OA3D_RENDERER_H
#define OA3D_RENDERER_H

#include "IRenderer.h"
#include "Model.h"
#include "Shader.h"

#include <iostream>


// Windoze or Linux debugger detach / break
#ifdef _MSC_VER
#define ASSERT(x) if (!(x)) __debugbreak();
#else
#define ASSERT(x) if (!(x)) __builtin_trap();
#endif

#define GLDebug(x) GLClearErrors();\
    x;\
    ASSERT(GLError(#x, __FILE__, __LINE__))

void GLClearErrors();
bool GLError(const char* function, const char* file, int line);

// Singleton
class Renderer : public IRenderer
{

    public:
       static Renderer* getInstance();

       void drawMesh(Mesh mesh, Shader shader) override;
       void draw(Entity entity, Shader shader) override;
       void drawModel(Model model, Shader shader) override;
       void drawInstanced(Entity entity, Shader shader) override;

       void getVAO(GLint *vao);
       void enableGLOptions();

    // Singleton
    private:
       Renderer(){};
       Renderer(Renderer const&);
       Renderer& operator=(Renderer const&);
       static Renderer* m_Instance;

       GLuint currVao;
};

#endif //OA3D_RENDERER_H

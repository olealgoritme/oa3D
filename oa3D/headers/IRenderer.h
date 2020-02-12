#ifndef _IRenderer_H
#define _IRenderer_H

#include "Model.h"
#include "Entity.h"

/*
┌───────────┐
│ IRenderer │
└───────────┘
- Renderer interface
- For use with different backends
- E.g OpenGL, Vulkan, etc
 */

class IRenderer
{

    private:
       static IRenderer* m_Instance;

    public:
       static IRenderer* getInstance();

       virtual void drawMesh(Mesh mesh, Shader shader) = 0; // must be implemented
       virtual void drawModel(Model model, Shader shader) = 0; // must be implemented
       //virtual void drawModelInstanced(Model model, Shader &shader) = 0; // must be implemented
       virtual void draw(Entity entity, Shader shader) = 0; // must be implemented
       virtual void drawInstanced(Entity entity, Shader shader) = 0; // must be implemented
       //virtual void drawPrimitive(Primitives primitive, Shader &shader) = 0; // must be implemented

};








#endif // _IRenderer_H


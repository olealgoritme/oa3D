#ifndef GUI_LOADER_H
#define GUI_LOADER_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Shader.h"
#include "Plane.h"
#include "IFileSaveCallback.h"
#include "IGUICallback.h"
#include "Model.h"
#include "GLFW/glfw3.h"
#include <vector>

class GUILoader
{

private:
    const char* GLSL_VERSION = "#version 460";

    void setupGuiStyle(bool styleDark, float alpha);

    IGUICallback *guiCallback;
    std::vector<Shader*> shaders;
    std::vector<Entity*> entities;

    void *mem_block; //memory block for memory editor
    void bindMVP(Entity *entity);
    void showStats();

public:

    GUILoader() {};
    ~GUILoader() {};
    void init(GLFWwindow *window);
    void render();
    void showEditor();
    void addShaders(std::vector<Shader*> &shaders);
    void setShaderFileCallback(IFileSaveCallback *fsCallback);
    void addEntities(std::vector<Entity*> &entities);
};



#endif

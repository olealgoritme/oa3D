#include "OA3D.h"
#include "GUILoader.h"
#include "imgui_mem.h"
#include "Editor.h"

#include <ctype.h>          // toupper
#include <limits.h>         // INT_MIN, INT_MAX
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>         // intptr_t
#else
#include <stdint.h>         // intptr_t
#endif

#include <vector>

bool SHOW_ZEP = true;

static Editor editor;

void GUILoader::init(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    //io.Fonts->AddFontFromFileTTF("../oa3D/oa3D/assets/dejavusansmono.ttf", 28);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;

    bool styleDark = true;
    float alpha = .9f;
    this->setupGuiStyle(styleDark, alpha);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(this->GLSL_VERSION);

    editor.registerSyntax();
}

void GUILoader::addShaders(std::vector<Shader*> &shaders)
{
     this->shaders = shaders;
     std::vector<const char *> files;

     for(unsigned int i = 0; i < this->shaders.size(); i++)
     {
        files.push_back(this->shaders[i]->vertexPath);
        files.push_back(this->shaders[i]->fragmentPath);
     }
     editor.addFiles(files);
     this->mem_block = (void *) &shaders;
}

void GUILoader::setShaderFileCallback(IFileSaveCallback *fsCallback)
{
     editor.setShaderFileCallback(fsCallback);
}

void GUILoader::addEntities(std::vector<Entity *> &entities)
{
     this->entities = entities;
}

void GUILoader::bindMVP(Entity *entity)
{
    bool firstUse = false;
    ImGui::SetWindowSize(ImVec2(800, 800));
    std::string title = "Scene tweaks (";
    title += std::to_string(entity->shader.programId).c_str();

    ImGui::Begin(title.c_str(), &firstUse, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);
    ImGui::NewLine();

    // Position
    ImGui::Text("Position");
    ImGui::SliderFloat("position.x", &entity->position.x, -1.0f, 1.0f);
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
        entity->position.x = 0.0f;

    ImGui::SliderFloat("position.y", &entity->position.y, -1.0f, 1.0f);
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
        entity->position.y = 0.0f;


    ImGui::SliderFloat("position.z", &entity->position.z, -1.0f, 1.0f);
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
        entity->position.z = 0.0f;

    ImGui::Text("Rotation");
    ImGui::SliderFloat("rotation.x", &entity->rotation.x, -1.0f, 1.0f);
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
        entity->rotation.x = 0.0f;

    // Rotation
    ImGui::SliderFloat("rotation.y", &entity->rotation.y, -1.0f, 1.0f);
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
        entity->rotation.y = 0.0f;

    ImGui::SliderFloat("rotation.z", &entity->rotation.z, -1.0f, 1.0f);
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
        entity->rotation.z = 0.0f;

    ImGui::SliderFloat("rotation.angle", &entity->rotationAngle, -360.0f, 360.0f);
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
        entity->rotationAngle = 0.0f;

    // Scaling
    ImGui::Text("Scaling");
    ImGui::SliderFloat("scaling.x", &entity->scaling.x, 0.0f, 10.0f);
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
        entity->scaling.x = 0.0f;

    ImGui::SliderFloat("scaling.y", &entity->scaling.y, 0.0f, 10.0f);
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
        entity->scaling.y = 0.0f;

    ImGui::SliderFloat("scaling.z", &entity->scaling.z, 0.0f, 10.0f);
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
        entity->scaling.z = 0.0f;

        ImGui::End();
}

void GUILoader::showStats()
{
    bool firstUse = false;
    ImGui::SetWindowSize(ImVec2(600, 200));
    ImGui::Begin("OA3D Stats", &firstUse, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);
    ImGui::NewLine();
    ImGui::Separator();
    ImGui::Text("[Stats] avg %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}



void GUILoader::render()
{

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        bool firstUse;

        // Layout
        for(unsigned int i = 0; i < entities.size(); i++)
            bindMVP(entities[i]);

        showStats();

        // Memory Editor
        ImGui::SetWindowPos(ImVec2( (mWidth * 0.75), 25));
        ImGui::SetWindowSize(ImVec2( mWidth * 0.22, 0.07));
        static MemoryEditor mem_edit_1;
        mem_edit_1.DrawWindow("Memory Editor", &mem_block, sizeof(mem_block));

        // Realtime editor
        firstUse = false;
        ImGui::SetWindowSize(ImVec2(600, 400));
        ImGui::Begin("OA3D Realtime Shader Editor", &firstUse, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar);

        showEditor();

        auto min = ImGui::GetCursorScreenPos();
        auto max = ImGui::GetContentRegionAvail();

        max.x = min.x + max.x;
        max.y = min.y + max.y;

        editor.GetEditor().RefreshRequired();  // required each frame
        editor.GetEditor().SetDisplayRegion(Zep::NVec2f(min.x, min.y), Zep::NVec2f(max.x, max.y));
        editor.GetEditor().Display();

        if (ImGui::IsWindowFocused())
        {
            editor.editor.HandleInput();
        }

        ImGui::End();

        // Render ImGui layout
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



}

void GUILoader::setupGuiStyle(bool styleDark, float alpha)
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.FrameRounding = 5.0f;
    style.Colors[ImGuiCol_Text]                  = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(1.00f, 0.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Header]                = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

    if(styleDark)
    {
        for (int i = 0; i <= ImGuiCol_COUNT; i++)
        {
            ImVec4& col = style.Colors[i];
            float H, S, V;
            ImGui::ColorConvertRGBtoHSV( col.x, col.y, col.z, H, S, V );

            if( S < 0.1f )
            {
                V = 1.0f - V;
            }
            ImGui::ColorConvertHSVtoRGB( H, S, V, col.x, col.y, col.z );
            if( col.w < 1.00f )
            {
                col.w *= alpha;
            }
        }
    }
    else
    {
        for (int i = 0; i <= ImGuiCol_COUNT; i++)
        {
            ImVec4& col = style.Colors[i];
            if( col.w < 1.00f )
            {
                col.x *= alpha;
                col.y *= alpha;
                col.z *= alpha;
                col.w *= alpha;
            }
        }
    }
}



void GUILoader::showEditor()
{

    // Simple menu options for switching mode and splitting
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Settings"))
        {
            if (ImGui::BeginMenu("Theme"))
            {
                bool enabledDark = editor.GetEditor().GetTheme().GetThemeType() == Zep::ThemeType::Dark ? true : false;
                bool enabledLight = !enabledDark;

                if (ImGui::MenuItem("Dark", "", &enabledDark))
                {
                    editor.GetEditor().GetTheme().SetThemeType(Zep::ThemeType::Dark);
                }
                else if (ImGui::MenuItem("Light", "", &enabledLight))
                {
                    editor.GetEditor().GetTheme().SetThemeType(Zep::ThemeType::Light);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

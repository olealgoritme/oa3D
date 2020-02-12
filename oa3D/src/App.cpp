// Local OA 3D Engine Headers
#include "OA3D.h"
#include "GLFW_backend.h"
#include "Shader.h"
#include "Mesh.h"
#include "IShaderCallback.h"
#include "Plane.h"

#include "glm/glm.hpp"

#include "IFileSaveCallback.h"
#include "RendererGL.h"
#include "Camera.h"
#include "GUILoader.h"
#include "Vertex.h"
#include "Skybox.h"
#include "Entity.h"
#include "Model.h"

// System Headers
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <vector>


class App : OA3D::GLFW::ICallback, IFileSaveCallback
{

public:

    // OA3D
    Renderer *renderer;
    GUILoader gui;
    std::vector<Shader*> shaders;
    std::vector<Entity*> entities;

    GLFWcursor *cursor;
    OA3D::GLFW::GLFWbackend *backend;
    OA3D::GLFW::Screen screen;

    // camera
    Camera camera;

    // mouse
    float lastX = mWidth / 2.0f;
    float lastY = mHeight / 2.0f;


    // Previous keys
    int oldAction = 0;
    int oldKey = 0;

    bool orbitCamera = true;
    bool firstMouse = true;

    // FPS / time
    float currentFrame;
    float lastFrame;
    float deltaTime; // between current and last frame
    int numFrames = 0;

    // instance object attributes
    bool INSTANCED = false;
    glm::vec2 translations[100];

    App() {

        screen = backend->initGLFW(mWidth, mHeight, this);

        if (screen.window == nullptr) {
            printf("Failed to Create OpenGL Context");
            return;
        }


        // camera
        camera = Camera();

        // GLAD loading OpenGL functions
        gladLoadGL();

        printf("OA3D OpenGL 3D Rendering Engine, OpenGL version %s\n", glGetString(GL_VERSION));
        printf("Written by Ole Algoritme (C) 2020\n");

        // tell GLFW to capture our mouse
        glfwSetInputMode(screen.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        // Initialize cursor in window
        init_cursor(screen.window);

        // OpenGL Option settings
        renderer->enableGLOptions();

        // FPS
        lastFrame = glfwGetTime();

        // Vsync off
        // glfwSwapInterval(0);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClearColor(.3, .3, .3, 1.0);

        // Init imgui
        gui = GUILoader();
        gui.init(screen.window);
        gui.setShaderFileCallback(this);

    }



    void instance_setup()
    {

            int index = 0;
            float offset = .1f;

            for (int y = -10; y < 10; y += 2)
                {
                for (int x = -10; x < 10; x += 2)

                    {
                        glm::vec2 translation;
                        translation.x = x / 10. + offset;
                        translation.y = y / 10. + offset;
                        translations[index++] = translation;
                    }
                }

            // store instance data in vbo
            unsigned int instanceVBO;

            glGenBuffers(1, &instanceVBO);
            glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW );

            // set instance data
            glBindVertexArray(1); // hoping it's at index 1

            glEnableVertexAttribArray(5);
            glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
            glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);
            glVertexAttribDivisor(5, 1);

            // done
            glBindVertexArray(0);
    }


    // create cursor
    void init_cursor(GLFWwindow* window)
    {
        // Creating a 8x8 pixel cursor from an arbitrary image
        // The image data is 32-bit, little-endian, non-premultiplied RGBA, i.e. eight bits per channel with the red channel first.
        // The pixels are arranged canonically as sequential rows, starting from the top-left corner.

        unsigned char pixels[8 * 8 * 4]; // 8 pixels by 8 pixels by 4 channels (RGBA - Red/Green/Blue/Alpha)
        memset(pixels, 0xff, sizeof(pixels));

        GLFWimage image;
        image.width = 8;
        image.height = 8;
        image.pixels = pixels;

        cursor = glfwCreateCursor(&image, 0, 0);
        glfwSetCursor(window, cursor);
    }


    // updates transform matrix for mvp
    void updateLights(Shader shader)
    {
        // lightpos / viewpos
        shader.Use();
        shader.setVec3("lightPos", glm::vec3(0.3, 1.0, -1.0));
        shader.setVec3("viewPos", camera.Position);
    }

    void updateGenerics(Shader shader)
    {

        // iMouse
        double xpos, ypos;
        glfwGetCursorPos(screen.window, &xpos, &ypos);
        shader.Use();
        shader.setVec2("iMouse", (float) xpos, (float) ypos);

        // iTime
        float now = glfwGetTime();
        shader.Use();
        shader.setFloat("iTime", (float) now);

        // iResolution
        shader.Use();
        shader.setVec2("iResolution", (float) screen.width, (float) screen.height); // viewport resolution (in pixels)
    }

    // updates transform matrix for mvp
    void updateEntityMatrix(Entity entity, Shader shader)
    {
        glm::mat4 _projection = camera.GetPerspective();
        shader.Use();
        shader.setMat4("projection", _projection);

        glm::mat4 _view = camera.GetViewMatrix();
        shader.Use();
        shader.setMat4("view", _view);

        glm::mat4 _model = entity.getTransformationMatrix();
        shader.Use();
        shader.setMat4("model", _model);

        glm::vec3 cameraPos = camera.getPosition();
        shader.Use();
        shader.setVec3("cameraPos", cameraPos);
    }

    void updateScene(Entity scene, Shader sceneShader)
    {
        glm::mat4 _projection = camera.GetPerspective();
        sceneShader.Use();
        sceneShader.setMat4("projection", _projection);

        glm::mat4 _view = camera.GetViewMatrix();
        sceneShader.Use();
        sceneShader.setMat4("view", _view);
    }

    void cleanup()
    {
        // GLFW Window close
        glfwSetWindowShouldClose(screen.window, true);

        // Destroy cursor
        glfwDestroyCursor(cursor);

        // Destroy window
        glfwDestroyWindow(screen.window);

        // Terminate glfw bindings
        glfwTerminate();

        // Unbind VAO
        glBindVertexArray(0);
    }

    void onMouseScroll(OA3D::GLFW::Mouse mouse) override
    {
        camera.ProcessMouseScroll((float) mouse.scroll.yOffset);
    }

    void onMouseClick(OA3D::GLFW::Mouse mouse) override
    {
        if (mouse.button == GLFW_MOUSE_BUTTON_LEFT && mouse.action == GLFW_PRESS)
        {
            double xpos = 0, ypos = 0;
            backend->getCursorPos(&xpos, &ypos);
            camera.ProcessMouseLeftPress((float) (xpos - lastX), (float) (lastY - ypos), deltaTime);
        }
    }

    void onMouseCursor(OA3D::GLFW::Mouse mouse) override
    {
        if (firstMouse)
        {
            lastX = mouse.cursor.x;
            lastY = mouse.cursor.y;
            firstMouse = false;
        }

        float xoffset = mouse.cursor.x - lastX;
        float yoffset = lastY - mouse.cursor.y; // reversed since y-coordinates go from bottom to top

        lastX = mouse.cursor.x;
        lastY = mouse.cursor.y;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }

    void onKeyPress(OA3D::GLFW::Key key) override
    {
        // Exit
        if (key.key == GLFW_KEY_Q
            && key.action == GLFW_PRESS
            && oldKey == GLFW_MOD_CONTROL
            && oldAction == GLFW_PRESS)
        {
            oldKey = GLFW_KEY_SPACE;
            cleanup();
        }

        // Toggle camera
        if (key.key == GLFW_KEY_SPACE
            && key.action == GLFW_RELEASE
            && oldKey == GLFW_MOD_CONTROL
            && oldAction == GLFW_PRESS)
        {
            oldKey = GLFW_KEY_SPACE;

            printf("OAD::GLFW::CTRL+Space = Toggle Camera\n");

                if(orbitCamera)
            {
                backend->setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                orbitCamera = false;
                camera.ToggleCamera();
            }
            else
            {
                backend->setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                orbitCamera = true;
                camera.ToggleCamera();
            }
        }

        // CTRL
        if (key.key_scan_code == 37 && key.action == GLFW_PRESS)
        {
            oldKey = GLFW_MOD_CONTROL;
            oldAction =  GLFW_PRESS;
        }

    }

    void onFrameBufferResize(OA3D::GLFW::Screen screen) override
    {
        glViewport(0, 0, screen.width, screen.height);
    }

    void onFileSaved() override
    {
        printf("APP::onFileSaved() CALLED!\n");

         for (unsigned int i = 0; i < this->shaders.size(); i++)
            this->shaders[i] = this->shaders[i]->ReloadShader(this->shaders[i]);
    }

    void processInputKeys()
    {
        // Forward
        if (glfwGetKey(screen.window, GLFW_KEY_W) == GLFW_PRESS && !orbitCamera)
        {
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }

        // Left
        if (glfwGetKey(screen.window, GLFW_KEY_A) == GLFW_PRESS && !orbitCamera)
        {
            camera.ProcessKeyboard(LEFT, deltaTime);
        }

        // Backward
        if (glfwGetKey(screen.window, GLFW_KEY_S) == GLFW_PRESS && !orbitCamera)
        {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }

        // Right
        if (glfwGetKey(screen.window, GLFW_KEY_D) == GLFW_PRESS && !orbitCamera)
        {
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }
    }

};


int main() {

    // App instance
    App *app = new App();

    // Get our OA3D Renderer
    app->renderer = app->renderer->getInstance();

    // Set viewport
    glViewport(0, 0, mWidth, mHeight);

    // Model shader
    Shader shader = Shader("../oa3D/oa3D/shaders/model_loading_vertex.glsl", "../oa3D/oa3D/shaders/model_loading_fragment.glsl");
    shader.Use();
    app->shaders.push_back(&shader);

    // Plane shader
    Shader planeShader = Shader("../oa3D/oa3D/shaders/plane_vert.glsl", "../oa3D/oa3D/shaders/plane_frag.glsl");
    planeShader.Use();
    app->shaders.push_back(&planeShader);

    // Skybox shader
    Shader skyShader = Shader("../oa3D/oa3D/shaders/skybox_vert.glsl", "../oa3D/oa3D/shaders/skybox_frag.glsl");
    skyShader.Use();
    skyShader.setInt("skybox", 0);
    app->shaders.push_back(&skyShader);

    // Scene shader
    Shader sceneShader = Shader("../oa3D/oa3D/shaders/scene_vert.glsl", "../oa3D/oa3D/shaders/scene_frag.glsl");
    sceneShader.Use();
    app->shaders.push_back(&sceneShader);

    // Add shaders to GUI editor for editing
    app->gui.addShaders(app->shaders);


    // Model coords
    float
    modelPosX = 0.0f,
    modelPosY = 0.1f,
    modelPosZ = 35.0f,
    modelScale = 1.0f;

    // MODEL matrix
    glm::vec3 pos, scale, rot;
    float rotAngle = 0;
    pos = glm::vec3(modelPosX, modelPosY, modelPosZ);
    rot = glm::vec3(0.0f, 1.0f, 0.0f);
    rotAngle = 180.0f;
    scale = glm::vec3(modelScale);

    // Model
    Model model = Model("../oa3D/oa3D/assets/nanosuit2/nanosuit.obj");
    model.setTransformation(pos, scale, rot, rotAngle);

    // PLANE matrix
    pos = glm::vec3(-0.7, 10.0, 75.0);
    scale = glm::vec3(39.0, 20.0, 0.0);
    rot = glm::vec3(1.0, 0.0, 0.0);
    rotAngle = 0.0;

    // Plane
    Plane plane = Plane();
    plane.setTransformation(pos, scale, rot, rotAngle);


    // Skybox matrix
    pos = glm::vec3(0.0, 0.0, 0.0);
    scale = glm::vec3(100.0, 100.0, 100.0);
    rot = glm::vec3(0.0, 0.0, 0.0);
    rotAngle = 0.0;

    // Skybox
    Skybox skybox = Skybox(true);
    skybox.setTransformation(pos, scale, rot, rotAngle);


    // Scene matrix
    pos = glm::vec3(0.0, -3.0, 0.0);
    scale = glm::vec3(500.0, 500.0, 500.0);
    rot = glm::vec3(0.0, 0.0, 0.0);
    rotAngle = 0.0;

    Model scene = Model("../oa3D/oa3D/assets/spacestation/spacestationscene.obj");
    scene.setTransformation(pos, scale, rot, rotAngle);


    app->entities.push_back(&plane);
    app->entities.push_back(&model);
    app->entities.push_back(&skybox);
    app->entities.push_back(&scene);
    app->gui.addEntities(app->entities);

    // Main render loop
    while (!app->backend->windowShouldClose()) {

        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Frame counter
        app->numFrames++;

        // Timing
        app->currentFrame = glfwGetTime();
        app->deltaTime = app->currentFrame - app->lastFrame;
        app->lastFrame = app->currentFrame;

        {

           // Plane
           planeShader.Use();
           app->updateEntityMatrix(plane, planeShader);
           app->updateGenerics(planeShader);
           app->renderer->draw(plane, planeShader);

           // Model
           shader.Use();
           app->updateEntityMatrix(model, shader);
           app->updateGenerics(shader);
           app->updateLights(shader);
           app->renderer->drawModel(model, shader);

           // Scene
           sceneShader.Use();
           app->updateScene(scene, sceneShader);
           app->renderer->drawModel(scene, sceneShader);

           // skybox
           skyShader.Use();
           skybox.drawSkyBox(&app->camera, &skyShader);

           // draw gui
           app->gui.render();

        }

        // Flip/Flush Buffers
        if(V_SYNC)
            glfwSwapBuffers(app->screen.window);
        else
            glFlush();

        // reset shader program usage
        glUseProgram(0);

        // Process rapid key presses
        app->processInputKeys();

        // Poll window system for events
        glfwPollEvents();
    }
}

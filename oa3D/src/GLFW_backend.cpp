#include "GLFW_backend.h"

#define V_SYNC false
#define AA_SAMPLES 4

static OA3D::GLFW::ICallback* _callBacks = NULL;
static OA3D::GLFW::Screen screen;
static OA3D::GLFW::Mouse mouse;
static OA3D::GLFW::Key keys;

static void init_callbacks();

OA3D::GLFW::Screen OA3D::GLFW::GLFWbackend::initGLFW(int width, int height, OA3D::GLFW::ICallback *callBacks)
{
    _callBacks = callBacks;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, AA_SAMPLES); // 4 x AA
    glfwWindowHint(GLFW_DOUBLEBUFFER, V_SYNC);
    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    screen.width = width;
    screen.height = height;
    screen.window = glfwCreateWindow(screen.width, screen.height, "OA3D OpenGL 3D Rendering Engine", nullptr, nullptr);


    glfwMakeContextCurrent(screen.window);

    init_callbacks();

    return screen;
}


void OA3D::GLFW::GLFWbackend::getCursorPos(double *xpos, double *ypos)
{
    return glfwGetCursorPos(screen.window, xpos, ypos);
}

bool OA3D::GLFW::GLFWbackend::windowShouldClose()
{
    return glfwWindowShouldClose(screen.window);
}

void OA3D::GLFW::GLFWbackend::setInputMode(int cursor, int cursorMode)
{
    return glfwSetInputMode(screen.window, cursor, cursorMode);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    screen.window = window;
    screen.width = width;
    screen.height = height;
    _callBacks->onFrameBufferResize(screen);
}

static void mouse_cursorPos_callback(GLFWwindow* window, double xpos, double ypos)
{
    (void) window;
    mouse.cursor.x = (float) xpos;
    mouse.cursor.y = (float) ypos;
    _callBacks->onMouseCursor(mouse);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    (void) window;
    mouse.button = button;
    mouse.action = action;
    mouse.mods = mods;
   _callBacks->onMouseClick(mouse);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    (void) window;
    mouse.scroll.xOffset = (float) xoffset;
    mouse.scroll.yOffset = (float) yoffset;
    _callBacks->onMouseScroll(mouse);
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void) window;
    keys.key = key;
    keys.key_scan_code = scancode;
    keys.action = action;
    keys.mods = mods;
    _callBacks->onKeyPress(keys);

}


static void init_callbacks()
{
    glfwSetFramebufferSizeCallback(screen.window, framebuffer_size_callback);
    glfwSetCursorPosCallback(screen.window, mouse_cursorPos_callback);
    glfwSetMouseButtonCallback(screen.window, mouse_button_callback);
    glfwSetScrollCallback(screen.window, scroll_callback);
    glfwSetKeyCallback(screen.window, key_callback);
}

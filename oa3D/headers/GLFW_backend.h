#ifndef OA3D_INPUT_H
#define OA3D_INPUT_H

#define V_SYNC false

#include "GLFW/glfw3.h"

namespace OA3D
{
    namespace GLFW
    {

        struct Cursor {
            float x;
            float y;
            int mode;
        };

        struct Scroll {
            float xOffset;
            float yOffset;
        };

        struct Mouse {
            int button;
            Cursor cursor;
            Scroll scroll;
            int action;
            int mods;
        };

        struct Key {
            int key;
            int action;
            int key_scan_code;
            int mods;
        };


        struct Screen {
            GLFWwindow *window;
            int width;
            int height;
        };


        class ICallback
        {

        public:
            virtual void onMouseScroll(OA3D::GLFW::Mouse mouse) = 0;
            virtual void onMouseClick(OA3D::GLFW::Mouse mouse) = 0;
            virtual void onMouseCursor(OA3D::GLFW::Mouse mouse) = 0;
            virtual void onKeyPress(OA3D::GLFW::Key key) = 0;
            virtual void onFrameBufferResize(OA3D::GLFW::Screen screen) = 0;
        };


        class GLFWbackend
        {
            public:
                Screen initGLFW(int width, int height, ICallback *callBacks);
                void getCursorPos(double *xpos, double *ypos);
                bool windowShouldClose();
                void setInputMode(int cursor, int cursorMode);
        };
    }

}

#endif

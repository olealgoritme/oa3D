#ifndef _Line_H
#define _Line_H

#include "Primitives.h"

class Line : Primitives
{

    public:
       void Draw(Shader &shader) override
       {
            this->shader = shader;
            glBegin(GL_QUADS);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex2f(this->xpos, this->ypos);
            glEnd();
       }

       void setData(float xpos, float ypos)
       {
            this->xpos = xpos;
            this->ypos = ypos;
       }

    private:
       float xpos;
       float ypos;
       Shader shader;
};


#endif // _Line_H


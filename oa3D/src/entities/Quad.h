#ifndef _Quad_H
#define _Quad_H

#include "Primitives.h"

class Quad : Primitives
{
    public:
        Quad();
       ~Quad();

       void Draw(Shader &shader) override
       {
            this->shader = shader;
       }

    private:

       Shader shader;
};


#endif // _Quad_H


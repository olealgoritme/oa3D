//
// Created by xuw on 19.01.2020.
//

#ifndef OA3D_VERTEXARRAY_H
#define OA3D_VERTEXARRAY_H

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{

public:
    VertexArray();
    ~VertexArray();

    void addBuffer(VertexBuffer& bf, const VertexBufferLayout& layout);

    void Bind();
    void Unbind();

private:
    unsigned int m_RendererID;
};


#endif //OA3D_VERTEXARRAY_H

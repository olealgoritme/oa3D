//
// Created by xuw on 17.01.2020.
//

#ifndef OA3D_VERTEXBUFFER_H
#define OA3D_VERTEXBUFFER_H

#include "Vertex.h"
#include <vector>

class VertexBuffer {

public:
    VertexBuffer();
    VertexBuffer(std::vector<Vertex> vertices);
    ~VertexBuffer();

    void Bind();
    void Unbind();
    int size();

private:
    unsigned int m_RendererID;
    std::vector<Vertex> vertices;
};


#endif

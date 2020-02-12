//
// Created by xuw on 19.01.2020.
//

#pragma once
#ifndef OA3D_VERTEXBUFFERLAYOUT_H
#define OA3D_VERTEXBUFFERLAYOUT_H

#include <vector>
#include <glad/glad.h>
#include "RendererGL.h"
#include "Vertex.h"
#include "VertexBuffer.h"


template<typename T>

struct identity {
    typedef T type;
};


struct VertexBufferElement
{
    const VertexBuffer vbo;
    VERTEX_TYPE vertex_type;     // vertex buffer type (position, normal, texcoords, tangent, bitangent)
    int vertex_count;            // number of vertices in this buffer
    unsigned int glType;         // GL_FLOAT / GL_UNSIGNED_INT / GL_UNSIGNED_BYTE
    unsigned char glNormalized;  // GL_TRUE / GL_FALSE


    // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    static unsigned int getSizeOfType(VERTEX_TYPE type)
    {
        switch (type)
        {
            case VERTEX_TYPE_POSITION  : return 0;
            case VERTEX_TYPE_NORMAL    : return offsetof(Vertex, Normal);
            case VERTEX_TYPE_TEXCOORDS : return offsetof(Vertex, TexCoords);
            case VERTEX_TYPE_TANGENT   : return offsetof(Vertex, Tangent);
            case VERTEX_TYPE_BITANGENT : return offsetof(Vertex, Bitangent);
        }
        return 0;
    }
};

class VertexBufferLayout
{

private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride; // space between two tuples (stride)

    template <typename T>
    void push(identity<T>, VertexBuffer vbo, VERTEX_TYPE vertex_type)
    {
        (void) vbo;
        (void) vertex_type;
    }

    void push(identity<float>, VertexBuffer vbo, VERTEX_TYPE vertex_type)
    {
        m_Elements.push_back({ vbo, vertex_type, vbo.size(), GL_FLOAT, GL_FALSE });
        m_Stride += VertexBufferElement::getSizeOfType(vertex_type) * vbo.size();
    }

    void push(identity<unsigned int>, VertexBuffer vbo, VERTEX_TYPE vertex_type)
    {
        m_Elements.push_back({ vbo, vertex_type, vbo.size(), GL_UNSIGNED_INT, GL_FALSE });
        m_Stride += VertexBufferElement::getSizeOfType(vertex_type) * vbo.size();
    }

    void push(identity<unsigned char>, VertexBuffer vbo, VERTEX_TYPE vertex_type)
    {
        m_Elements.push_back({ vbo, vertex_type, vbo.size(), GL_UNSIGNED_BYTE, GL_TRUE });
        m_Stride += VertexBufferElement::getSizeOfType(vertex_type) * vbo.size();
    }

public:
    VertexBufferLayout()
    : m_Stride(0) {};

    template <typename T>
    void push(VertexBuffer vbo, VERTEX_TYPE vertex_type)
    {
        push(identity<T>(), vbo, vertex_type);
    }

    inline const std::vector<VertexBufferElement> &getElements() const { return m_Elements; }
    inline unsigned int getStride() const { return m_Stride; }
};



#endif //OA3D_VERTEXBUFFERLAYOUT_H

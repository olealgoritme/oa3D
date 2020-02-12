//
// Created by xuw on 17.01.2020.
//

#include "glad/glad.h"
#include "VertexBuffer.h"

#include <cstdio>

VertexBuffer::VertexBuffer(std::vector<Vertex> vertices)
{
    this->vertices = vertices;
    glGenBuffers( 1, &this->m_RendererID);
    glBindBuffer( GL_ARRAY_BUFFER, this->m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, this->size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
}

int VertexBuffer::size() {
    return this->vertices.size();
}

VertexBuffer::VertexBuffer() {
    // Empty constructor
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers( 1, (unsigned int *) &this->m_RendererID );
}


void VertexBuffer::Bind()
{
    glBindBuffer( GL_ARRAY_BUFFER, this->m_RendererID );
    printf("Binding VBO [ID]: %d", this->m_RendererID);
}

void VertexBuffer::Unbind()
{
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    printf("Binding VBO [ID]: %d", this->m_RendererID);
}

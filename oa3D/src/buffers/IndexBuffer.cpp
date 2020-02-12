//
// Created by xuw on 17.01.2020.
//

#include "glad/glad.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer() {
    // empty constructor
}

IndexBuffer::IndexBuffer(std::vector<unsigned int> indices)
{
    glGenBuffers( 1, &this->m_RendererID);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers( 1, &this->m_RendererID );
}


void IndexBuffer::Bind() const
{
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->m_RendererID );
}

void IndexBuffer::Unbind() const
{
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

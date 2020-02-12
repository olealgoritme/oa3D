//
// Created by xuw on 19.01.2020.
//

#include "glad/glad.h"
#include "VertexArray.h"

#include <cstdio>

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

/*
 * VertexArray binds VertexArray and all child VertexBuffer elements (and their layout)
 * */

VertexArray::VertexArray()
{
    glGenVertexArrays( 1, &this->m_RendererID );
    printf("Generating VBA [ID]: %d", this->m_RendererID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays( 1, (unsigned int *) &this->m_RendererID );
}

void VertexArray::addBuffer(VertexBuffer &vbo, const VertexBufferLayout &layout)
{
    // Bind Vertex Array
    this->Bind();

    // Bind Vertex Buffer
    vbo.Bind();

    const auto &elements = layout.getElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++)
    {
       const auto &element = elements[i];

       // Vertex Layout
        glEnableVertexAttribArray( i );
        glVertexAttribPointer( i, element.vertex_count, element.glType, element.glNormalized, layout.getStride(), (const void *) INT2VOIDP(offset));
        offset += element.vertex_count * VertexBufferElement::getSizeOfType(element.vertex_type);
    }
}


void VertexArray::Bind()
{
    glBindVertexArray( this->m_RendererID );
}
void VertexArray::Unbind()
{
    glBindVertexArray( 0 );
}

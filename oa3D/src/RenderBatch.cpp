#include "RenderBatch.h"

// Instance reference
RenderBatch* RenderBatch::m_Instance = nullptr;

RenderBatch* RenderBatch::getInstance()
{
    if(!m_Instance)
        m_Instance = new RenderBatch();

    return m_Instance;
}


// Idea:
// only use ONE SINGLE VAO (Vertex Array Object)
// concatenate all VBO's into large VBO, with offets, and only ONE SINGLE VAO
// need good logic for this


#ifndef VERTEX_H
#define VERTEX_H

#include <string>
#include <vector>
#include "glm/glm.hpp"

// Type of vertex
enum VERTEX_TYPE {
        VERTEX_TYPE_POSITION,
        VERTEX_TYPE_COLOR,
        VERTEX_TYPE_NORMAL,
        VERTEX_TYPE_TEXCOORDS,
        VERTEX_TYPE_TANGENT,
        VERTEX_TYPE_BITANGENT,
        VERTEX_TYPE_UV
};

// Vertex structure
struct Vertex {
    // position
    glm::vec3 Position;
    // color
    glm::vec4 Color;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    // uv vec2
    glm::vec2 UV;
};

#endif

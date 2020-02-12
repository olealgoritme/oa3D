#ifndef _BoneMatrix_H
#define _BoneMatrix_H

#include "glm/glm.hpp"
#include "assimp/Importer.hpp"

struct BoneMatrix {
    aiMatrix4x4 offset_matrix;
    aiMatrix4x4 final_world_transformation;
};


#endif // _BoneMatrix_H


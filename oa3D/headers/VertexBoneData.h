#ifndef _VertexBoneData_H
#define _VertexBoneData_H

#define BONES_PER_VERTEX 4

#include <cstring>

struct VertexBoneData {

    unsigned int ids[BONES_PER_VERTEX];
    float weights[BONES_PER_VERTEX];

    VertexBoneData()
    {
        memset(ids, 0, sizeof(ids)); // reserve memory space for array
        memset(weights, 0, sizeof(weights));
    }

    void addBoneData(unsigned int bone_id, float weight) {
        for (unsigned int i = 0; i < BONES_PER_VERTEX; i++)
        {
            if (weights[i] == 0.0)
            {
                ids[i] = bone_id;
                weights[i] = weight;
                return;
            }
        }

    }

};


#endif // _VertexBoneData_H


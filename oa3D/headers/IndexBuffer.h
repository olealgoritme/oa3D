//
// Created by xuw on 17.01.2020.
//

#ifndef OA3D_INDEXBUFFER_H
#define OA3D_INDEXBUFFER_H

#include <vector>

class IndexBuffer {

private:
    unsigned int m_RendererID;
    unsigned int m_Count;

public:
    IndexBuffer(void);
    IndexBuffer(std::vector<unsigned int> indices);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const { return m_Count; }
};


#endif

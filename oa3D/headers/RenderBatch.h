#ifndef _RenderBatch_H
#define _RenderBatch_H

// Singleton
class RenderBatch
{

    public:
       static RenderBatch* getInstance();

    private:
       RenderBatch(){};
       RenderBatch(RenderBatch const&);
       RenderBatch& operator=(RenderBatch const&);
       static RenderBatch* m_Instance;
};


#endif // _RenderBatch_H


#ifndef _IMAGE_H_
#define _IMAGE_H_

class Image {
public:
    Image() : mImageData(nullptr) {}
    virtual ~Image();
    Image(const char *filename, int *x, int *y, int *comp, int req_comp);
    void free(unsigned char *data);

    unsigned char* getData() const { return mImageData; }
protected:
    unsigned char* mImageData;
};


#endif

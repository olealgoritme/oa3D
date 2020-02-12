#include "Image.h"

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// The return value from an image loader is an 'unsigned char *' which points
// to the pixel data, or NULL on an allocation failure or if the image is
// corrupt or invalid. The pixel data consists of *y scanlines of *x pixels,
// with each pixel consisting of N interleaved 8-bit components; the first
// pixel pointed to is top-left-most in the image. There is no padding between
// image scanlines or between pixels, regardless of format. The number of
// components N is 'desired_channels' if desired_channels is non-zero, or
// *channels_in_file otherwise. If desired_channels is non-zero,
// *channels_in_file has the number of components that _would_ have been
// output otherwise. E.g. if you set desired_channels to 4, you will always
// get RGBA output, but you can check *channels_in_file to see if it's trivially
// opaque because e.g. there were only 3 channels in the source image.


Image::~Image()
{
    if ( this->mImageData )
        stbi_image_free(this->mImageData);
}

Image::Image(const char *filename, int *x, int *y, int *comp, int req_comp)
{
    this->mImageData = stbi_load(filename, x, y, comp, req_comp);
}

void Image::free(unsigned char *data)
{
    stbi_image_free(data);
}


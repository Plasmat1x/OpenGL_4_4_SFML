#pragma once
#include <gl/glew.h>
#include "Image.h"

class Texture
{
public:
    unsigned int data;
    Image* image;

    Texture();
    Texture(Image image);
    ~Texture();

    bool createTexture(Image& image);
};


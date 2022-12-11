#pragma once
#include "stb_image.h"

#include <iostream>

class Image
{
public:
    unsigned char* data;

    int width;
    int height;
    int nrChannels;

    Image();
    Image(const char* imagePath);
    Image(const Image& image);
    Image(Image&& image);

    ~Image();

    bool loadImage(const char* imagePath);
    void clear();

    const Image& operator=(const Image& image);
    Image& operator=(Image&& image);
};


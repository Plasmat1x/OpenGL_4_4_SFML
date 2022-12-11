#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION

Image::Image()
{
    this->data = nullptr;
    this->width = 0;
    this->height = 0;
    this->nrChannels = 0;
}

Image::Image(const char* imagePath)
{
    loadImage(imagePath);
}

Image::Image(const Image& image)
{
    if (&image != this)
    {
        this->data = nullptr;
        this->width = 0;
        this->height = 0;
        this->nrChannels = 0;

        *this = image;
    }

}

Image::Image(Image&& image)
{
    if (&image != this)
    {
        this->data = image.data;
        this->width = image.width;
        this->height = image.height;
        this->nrChannels = image.nrChannels;

        image.clear();
        image.data = nullptr;
        image.width = 0;
        image.height = 0;
        image.nrChannels = 0;
    }
}

Image::~Image()
{
    clear();
}

bool Image::loadImage(const char* imagePath)
{
    data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    
    if(!data)
    {
        std::cout << "IMG::LOAD::ERR::" << imagePath << std::endl;
        return false;
    }
    return true;
}

void Image::clear()
{
    if (data)
    {
        delete data;   
    }

    data = nullptr;
}


const Image& Image::operator=(const Image& image)
{
    clear();

    this->data = image.data;
    this->width = image.width;
    this->height = image.height;
    this->nrChannels = image.nrChannels;

    return *this;
}

Image& Image::operator=(Image&& image)
{
    if (&image != this)
    {
        clear();

        this->data = image.data;
        this->width = image.width;
        this->height = image.height;
        this->nrChannels = image.nrChannels;

        image.clear();
        image.data = nullptr;
        image.width = 0;
        image.height = 0;
        image.nrChannels = 0;
    }

    return *this;
}
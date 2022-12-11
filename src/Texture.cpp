#include "Texture.h"

Texture::Texture()
{
    this->data = 0;
    this->image = nullptr;
}

Texture::Texture(Image image)
{
    createTexture(image);
}

Texture::~Texture()
{
    this->image->clear();
}

bool Texture::createTexture(Image& image)
{
    this->image = &image;

    if (this->image->data)
        return false;

    glGenTextures(1, &this->data);
    glBindTexture(GL_TEXTURE_2D, this->data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->image->width, this->image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->image->data);
    glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}

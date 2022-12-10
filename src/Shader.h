#pragma once

#include <GL/glew.h>

#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <thread>

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    ~Shader();

    void Use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

private:

    unsigned int ID;
};